/**
 * (C) Copyright 2020 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 "
 * GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
 * The Government's rights to use, modify, reproduce, release, perform, display,
 * or disclose this software are subject to the terms of the Apache License as
 * provided in Contract No. B609815.
 * Any reproduction of computer software, computer software documentation, or
 * portions thereof marked with this legend must also reproduce the markings.
 */
/**
 * DAOS server erasure-coded object aggregation.
 *
 * src/object/srv_ec_aggregate.c
 */
#define D_LOGFAC	DD_FAC(object)

#include <stddef.h>
#include <stdio.h>
#include <daos/common.h>
#include <daos_srv/vos.h>
#include <daos_srv/daos_server.h>
#ifndef VOS_UNIT_TEST
#include <daos_srv/srv_obj_ec.h>
#endif
#include "obj_ec.h"
#include "obj_internal.h"

/* Pool/container info. */
struct ec_agg_pool_info {
	uuid_t		 api_pool_uuid;		/* open pool, check leader */
	uuid_t		 api_poh_uuid;		/* pool handle uuid */
	uuid_t		 api_cont_uuid;		/* container uuid */
	uuid_t		 api_coh_uuid;		/* container handle uuid */
	daos_handle_t	 api_cont_hdl;		/* container handle */
	uint32_t	 api_pool_version;	/* pool vers,fo check leader*/
	d_rank_list_t	*api_svc_list;		/* service list */
	struct ds_pool	*api_pool;		/* Used for IV fetch */
};

/* Parameters used to drive iterate all.
 */
struct ec_agg_param {
	struct ec_agg_entry	*ap_agg_entry;	 /* entry used for each OID */
	d_sg_list_t		 ap_sgl;	 /* mem alloc-ed for I/O */
	struct ec_agg_pool_info	 ap_pool_info;	 /* pool/cont info */
	daos_handle_t		 ap_cont_handle; /* container handle */
};

/* Parity extent for the stripe undergoing aggregation.
 */
struct ec_agg_par_extent {
	daos_recx_t	ape_recx;	/* recx for the parity extent */
	daos_epoch_t	ape_epoch;	/* epoch of the parity extent */
};

/* Represents the current stripe undergoing aggregation.
 */
struct ec_agg_stripe {
	daos_off_t	as_stripenum;   /* ord of the stripe, offset/(k*len) */
	daos_epoch_t	as_hi_epoch;    /* highest epoch of extents in stripe */
	d_list_t	as_dextents;    /* list of stripe's data extents */
	daos_off_t	as_stripe_fill; /* amount of stripe covered by reps */
	unsigned int	as_extent_cnt;  /* number of replica extents */
	unsigned int	as_offset;     /* start offset in stripe */
};

/* Aggregation state for an object. (may need to restructure if
 * list of these is built from committed DTX table)
 */
struct ec_agg_entry {
	d_list_t		 ae_link;
	daos_unit_oid_t		 ae_oid;
	struct daos_oclass_attr	*ae_oca;
	struct obj_ec_codec	*ae_codec;
	d_sg_list_t		*ae_sgl;
	daos_handle_t		 ae_cont_hdl;
	daos_handle_t		 ae_chdl;
	daos_handle_t		 ae_thdl;
	daos_epoch_range_t	 ae_epr; /* hi/lo extent threshold */
	daos_key_t		 ae_dkey;
	daos_key_t		 ae_akey;
	daos_size_t		 ae_rsize;
	struct ec_agg_stripe	 ae_cur_stripe;
	struct ec_agg_par_extent ae_par_extent;
	daos_handle_t		 ae_obj_hdl;
};

/* Struct used to drive offloaded stripe update.
 */
struct ec_agg_stripe_ud {
	struct ec_agg_entry	*asu_agg_entry;
	uint8_t			*asu_bit_map;
	unsigned int		 asu_cell_cnt;
	bool			 asu_recalc;
	//ABT_eventual		 asu_eventual;
};

/* Represents an replicated data extent.
 */
struct ec_agg_extent {
	d_list_t	ae_link;
	daos_recx_t	ae_recx;
	daos_epoch_t	ae_epoch;
};

/* Reset iterator state upon completion of iteration of a subtree.
 */
static inline void
reset_agg_pos(vos_iter_type_t type, struct ec_agg_entry *agg_entry)
{
	switch (type) {
	case VOS_ITER_DKEY:
		memset(&agg_entry->ae_dkey, 0, sizeof(agg_entry->ae_dkey));
		break;
	case VOS_ITER_AKEY:
		memset(&agg_entry->ae_akey, 0, sizeof(agg_entry->ae_akey));
		break;
	default:
		break;
	}
}

/* Compare function for keys.  Used to reset iterator position.
 */
static inline int
agg_key_compare(daos_key_t key1, daos_key_t key2)
{
	if (key1.iov_len != key2.iov_len)
		return 1;

	return memcmp(key1.iov_buf, key2.iov_buf, key1.iov_len);
}

/* Handles dkeys returned by the per-object nested iteratior.
 */
static int
agg_dkey(daos_handle_t ih, vos_iter_entry_t *entry,
	 struct ec_agg_entry *agg_entry, unsigned int *acts)
{
	int rc = 0;

	if (agg_key_compare(agg_entry->ae_dkey, entry->ie_key)) {
		agg_entry->ae_dkey = entry->ie_key;
		reset_agg_pos(VOS_ITER_AKEY, agg_entry);
	}
	agg_entry->ae_dkey	= entry->ie_key;
	return rc;
}

/* Handles akeys returned by the per-object nested iteratior.
 */
static int
agg_akey(daos_handle_t ih, vos_iter_entry_t *entry,
	 struct ec_agg_entry *agg_entry, unsigned int *acts)
{
	int rc = 0;

	agg_entry->ae_akey	= entry->ie_key;
	agg_entry->ae_thdl	= ih;
	return rc;
}

/* Determines if the extent carries over into the next stripe.
 */
static bool
agg_carry_over(struct ec_agg_entry *agg_entry, struct ec_agg_extent *agg_extent)
{
	/* TBD */
	return false;
}

/* Clears the extent list of all extents completed for the processed stripe.
 * Extents the carry over to the next stripe have the prior-stripe prefix
 * trimmed (TBD).
 */
static void
agg_clear_extents(struct ec_agg_entry *agg_entry)
{
	struct ec_agg_extent *agg_extent, *ext_tmp;

	d_list_for_each_entry_safe(agg_extent, ext_tmp,
				   &agg_entry->ae_cur_stripe.as_dextents,
				   ae_link) {
		/* Check for carry-over extent. */
		if (!agg_carry_over(agg_entry, agg_extent)) {
			agg_entry->ae_cur_stripe.as_extent_cnt--;
			d_list_del(&agg_extent->ae_link);
			D_FREE_PTR(agg_extent);
		}
	}
	agg_entry->ae_cur_stripe.as_hi_epoch = 0UL;
	/* Should account for carry over. */
	agg_entry->ae_cur_stripe.as_stripe_fill = 0U;
}

/* Retrunns the stripe number for the stripe containing ex_lo.
 */
static inline daos_off_t
agg_stripenum(struct ec_agg_entry *entry, daos_off_t ex_lo)
{
	return ex_lo / (entry->ae_oca->u.ec.e_k * entry->ae_oca->u.ec.e_len);
}

/* Call back for the nested iterator used to find the parity for a stripe.
 */
static int
agg_recx_iter_pre_cb(daos_handle_t ih, vos_iter_entry_t *entry,
		     vos_iter_type_t type, vos_iter_param_t *param,
		     void *cb_arg, unsigned int *acts)
{
	struct ec_agg_entry	*agg_entry = (struct ec_agg_entry *) cb_arg;
	int			 rc = 0;

	D_ASSERT(type == VOS_ITER_RECX);
	D_ASSERT(entry->ie_recx.rx_idx == (PARITY_INDICATOR |
		(agg_entry->ae_cur_stripe.as_stripenum *
			agg_entry->ae_oca->u.ec.e_len)));
	agg_entry->ae_par_extent.ape_recx = entry->ie_recx;
	agg_entry->ae_par_extent.ape_epoch = entry->ie_epoch;
	return rc;
}

enum agg_iov_entry {
	AGG_IOV_DATA	= 0,
	AGG_IOV_ODATA,
	AGG_IOV_PARITY,
	AGG_IOV_DIFF,
	AGG_IOV_CNT,
};

/* Allocates an sgl iov_buf at iov_entry offset in the array.
 */
static int
agg_alloc_buf(d_sg_list_t *sgl, size_t ent_buf_len, unsigned int iov_entry,
	      bool align_data)
{
	int		 rc = 0;

	if (align_data) {
		D_FREE(sgl->sg_iovs[iov_entry].iov_buf);
		sgl->sg_iovs[iov_entry].iov_buf =
			aligned_alloc(32, ent_buf_len);
		if (sgl->sg_iovs[iov_entry].iov_buf == NULL) {
			rc = -DER_NOMEM;
			goto out;
		}
	} else {
		unsigned int *buf = NULL;
		 D_REALLOC(buf, sgl->sg_iovs[iov_entry].iov_buf,
			                     ent_buf_len);
		 if (buf == NULL) {
			rc = -DER_NOMEM;
			goto out;
		 }
		sgl->sg_iovs[iov_entry].iov_buf = buf;
	}
	sgl->sg_iovs[iov_entry].iov_len = ent_buf_len;
	sgl->sg_iovs[iov_entry].iov_buf_len = ent_buf_len;
out:
	return rc;
}

/* Prepares the SGL used for VOS I/O and peer target I/O (update needed
 * to use library functions (e.g., d_iov_set).
 *
 * This function is a no-op if entry's sgl is sufficient for the current
 * object class.
 *
 */
static int
agg_prep_sgl(struct ec_agg_entry *entry)
{
	size_t		 data_buf_len, par_buf_len;
	unsigned int	 len = entry->ae_oca->u.ec.e_len;
	unsigned int	 k = entry->ae_oca->u.ec.e_k;
	unsigned int	 p = entry->ae_oca->u.ec.e_p;
	int		 rc = 0;

	if (entry->ae_sgl->sg_nr == 0) {
		D_ALLOC_ARRAY(entry->ae_sgl->sg_iovs, AGG_IOV_CNT);
		if (entry->ae_sgl->sg_iovs == NULL) {
			rc = -DER_NOMEM;
			goto out;
		}
		entry->ae_sgl->sg_nr = AGG_IOV_CNT;
	}
	D_ASSERT(entry->ae_sgl->sg_nr == AGG_IOV_CNT);
	data_buf_len = len * k * entry->ae_rsize;
	if (entry->ae_sgl->sg_iovs[AGG_IOV_DATA].iov_buf_len < data_buf_len) {
		rc = agg_alloc_buf(entry->ae_sgl, data_buf_len, AGG_IOV_DATA,
				   true);
		if (rc)
			goto out;
	}
	if (entry->ae_sgl->sg_iovs[AGG_IOV_ODATA].iov_buf_len < data_buf_len) {
		rc = agg_alloc_buf(entry->ae_sgl, data_buf_len, AGG_IOV_ODATA,
				   true);
		if (rc)
			goto out;
	}
	if (entry->ae_sgl->sg_iovs[AGG_IOV_DIFF].iov_buf_len <
						len * entry->ae_rsize) {
		rc = agg_alloc_buf(entry->ae_sgl, len * entry->ae_rsize,
				   AGG_IOV_DIFF, true);
		if (rc)
			goto out;
	}
	par_buf_len = len * p * entry->ae_rsize;
	if (entry->ae_sgl->sg_iovs[AGG_IOV_PARITY].iov_buf_len < par_buf_len) {
		rc = agg_alloc_buf(entry->ae_sgl, par_buf_len, AGG_IOV_PARITY,
				   false);
		if (rc)
			goto out;
	}
out:
	return rc;

}

/* Recovers allocated memory for sgl used in the aggregation process.
 */
static void
agg_sgl_fini(d_sg_list_t *sgl)
{
	int i;

	if (sgl->sg_iovs) {
		for (i = 0; i < AGG_IOV_CNT; i++)
			D_FREE(sgl->sg_iovs[i].iov_buf);
		D_FREE(sgl->sg_iovs);
	}
}

/* Fetches the full data stripe (called when replicas form a full stripe).
 */
static int
agg_fetch_data_stripe(struct ec_agg_entry *entry)
{
	daos_iod_t	iod = { 0 };
	daos_recx_t	recx = { 0 };
	unsigned int	len = entry->ae_oca->u.ec.e_len;
	unsigned int	k = entry->ae_oca->u.ec.e_k;
	int		rc = 0;

	rc = agg_prep_sgl(entry);
	if (rc)
		goto out;
	recx.rx_idx = entry->ae_cur_stripe.as_stripenum * k * len;
	recx.rx_nr = k * len;
	iod.iod_name = entry->ae_akey;
	iod.iod_type = DAOS_IOD_ARRAY;
	iod.iod_size = entry->ae_rsize;
	iod.iod_nr = 1;
	iod.iod_recxs = &recx;
	entry->ae_sgl->sg_nr = 1;
	entry->ae_sgl->sg_iovs[AGG_IOV_DATA].iov_len =
						len * k * entry->ae_rsize;

	rc = vos_obj_fetch(entry->ae_chdl, entry->ae_oid,
			   entry->ae_cur_stripe.as_hi_epoch,
			   VOS_FETCH_RECX_LIST, &entry->ae_dkey, 1, &iod,
			   entry->ae_sgl);
	if (rc)
		D_ERROR("vos_obj_fetch failed: "DF_RC"\n", DP_RC(rc));
	entry->ae_sgl->sg_nr = AGG_IOV_CNT;
out:
	return rc;
}

/* Encodes a full stripe. Called when replicas form a full stripe.
 */
static void
agg_encode_full_stripe(void *arg)
{
	struct ec_agg_entry	*entry = (struct ec_agg_entry *)arg;
	unsigned int		 len = entry->ae_oca->u.ec.e_len;
	unsigned int		 k = entry->ae_oca->u.ec.e_k;
	unsigned int		 p = entry->ae_oca->u.ec.e_p;
	unsigned int		 cell_bytes = len * entry->ae_rsize;
	unsigned char		*data[k];
	unsigned char		*parity_bufs[p];
	unsigned char		*buf;
	int			 i;

	buf = entry->ae_sgl->sg_iovs[AGG_IOV_DATA].iov_buf;
	for (i = 0; i < k; i++)
		data[i] = &buf[i*cell_bytes];

	buf = entry->ae_sgl->sg_iovs[AGG_IOV_PARITY].iov_buf;
	for (i = p - 1; i >= 0; i--)
		parity_bufs[i] = &buf[i*cell_bytes];

	if (entry->ae_codec == NULL)
		entry->ae_codec =
		obj_ec_codec_get(daos_obj_id2class(entry->ae_oid.id_pub));
	ec_encode_data(cell_bytes, k, p, entry->ae_codec->ec_gftbls, data,
		       parity_bufs);
}

/* Driver function for full_stripe encode. Fetches the data and then invokes
 * second function to encode the parity.
 */
static int
agg_encode_local_parity(struct ec_agg_entry *entry)
{
	int rc = 0;

	rc = agg_fetch_data_stripe(entry);
	if (rc)
		goto out;
	agg_encode_full_stripe(entry);

out:
	return rc;
}

/* True if all extents within the stripe are at a higher epoch than
 * the parity for the stripe.
 */
static bool
agg_data_is_newer(struct ec_agg_entry *entry)
{
	struct ec_agg_extent	*agg_extent;

	d_list_for_each_entry(agg_extent, &entry->ae_cur_stripe.as_dextents,
			      ae_link) {
		if (agg_extent->ae_epoch < entry->ae_par_extent.ape_epoch)
			return false;
	}
	return true;
}

/* Determines if the replicas present for the current stripe of object entry
 * constitute a full stripe. If parity exists for the the stripe, the replicas
 * making up the full stripe must be a later epoch that the parity.
 */
static bool
agg_stripe_is_filled(struct ec_agg_entry *entry, bool has_parity)
{
	bool	is_filled, rc = false;


	is_filled = entry->ae_cur_stripe.as_stripe_fill ==
		entry->ae_oca->u.ec.e_k * entry->ae_oca->u.ec.e_len;

	if (is_filled)
		if (!has_parity || agg_data_is_newer(entry))
			rc = true;
	return rc;
}

/* Writes updated parity to VOS, and removes replicas fully contained
 * in the processed stripe.
 */
static int
agg_update_vos(struct ec_agg_entry *entry)
{
	d_sg_list_t		sgl = { 0 };
	daos_iod_t		iod = { 0 };
	daos_epoch_range_t	epoch_range = { 0 };
	d_iov_t			*iov;
	daos_recx_t		recx = { 0 };
	unsigned int		len = entry->ae_oca->u.ec.e_len;
	unsigned int		k = entry->ae_oca->u.ec.e_k;
	int			rc = 0;

	iov = &entry->ae_sgl->sg_iovs[AGG_IOV_PARITY];
	sgl.sg_iovs = iov;
	sgl.sg_nr = 1;

	recx.rx_idx = entry->ae_cur_stripe.as_stripenum * k * len;
	recx.rx_nr = k * len;
	epoch_range.epr_lo = 0ULL;
	epoch_range.epr_hi = entry->ae_cur_stripe.as_hi_epoch;
	rc = vos_obj_array_remove(entry->ae_chdl, entry->ae_oid,
				  &epoch_range, &entry->ae_dkey,
				  &entry->ae_akey, &recx);

	iod.iod_nr = 1;
	iod.iod_size = entry->ae_rsize;
	iod.iod_name = entry->ae_akey;
	iod.iod_type = DAOS_IOD_ARRAY;
	iod.iod_recxs = &recx;
	recx.rx_idx = entry->ae_cur_stripe.as_stripenum * len;
	recx.rx_nr = len;
	rc = vos_obj_update(entry->ae_chdl, entry->ae_oid,
			    entry->ae_cur_stripe.as_hi_epoch, 0, 0,
			    &entry->ae_dkey, 1, &iod, NULL,
			    &sgl);
	if (rc)
		D_PRINT("vos_obj_update failed: "DF_RC"\n", DP_RC(rc));
	return rc;
}

/* Process the prior stripe. Invoked when the iterator has moved to the first
 * extent in the subsequent.
 */
static int
agg_process_stripe(struct ec_agg_entry *entry)
{
	vos_iter_param_t	iter_param = { 0 };
	struct vos_iter_anchors	anchors = { 0 };
	bool			update_vos = true;
	int			rc = 0;

	entry->ae_par_extent.ape_epoch	= ~(0ULL);

	iter_param.ip_hdl		= DAOS_HDL_INVAL;
	iter_param.ip_ih		= entry->ae_thdl;
	iter_param.ip_flags		= VOS_IT_RECX_VISIBLE;
	iter_param.ip_recx.rx_idx	= PARITY_INDICATOR |
					  (entry->ae_cur_stripe.as_stripenum *
						entry->ae_oca->u.ec.e_len);
	iter_param.ip_recx.rx_nr	= entry->ae_oca->u.ec.e_len;

	D_DEBUG(DB_TRACE, "Querying parity for stripe: %lu, offset: %lu\n",
		entry->ae_cur_stripe.as_stripenum,
		iter_param.ip_recx.rx_idx);
	D_PRINT("Querying parity for stripe: %lu, offset: %lu\n",
		entry->ae_cur_stripe.as_stripenum,
		iter_param.ip_recx.rx_idx);
	rc = vos_iterate(&iter_param, VOS_ITER_RECX, false, &anchors,
			 agg_recx_iter_pre_cb, NULL, entry, NULL);
	if (rc != 0)
		goto out;

	if (entry->ae_par_extent.ape_epoch > entry->ae_cur_stripe.as_hi_epoch &&
			entry->ae_par_extent.ape_epoch != ~(0ULL)) {
		/* Parity newer than data; nothing to do. */
		update_vos = false;
		goto out;
	}

	if ((entry->ae_par_extent.ape_epoch == ~(0ULL)
				 && agg_stripe_is_filled(entry, false)) ||
					agg_stripe_is_filled(entry, true)) {
		/* Replicas constitute a full stripe. */
		D_PRINT("Encoding local parity\n");
		rc = agg_encode_local_parity(entry);
		D_PRINT("Encoding local parity returned %d\n", rc);
		goto out;
	}
	if (entry->ae_par_extent.ape_epoch == ~(0ULL)) {
		update_vos = false;
		goto out;
	}
out:
	if (update_vos && rc == 0)
		rc = agg_update_vos(entry);
		/* offload of ds_obj_update (TBD) to push remote parity */

	agg_clear_extents(entry);
	return rc;
}

/* returns the subrange of the RECX iterator's returned recx that lies within
 * the current stripe.
 */
static daos_off_t
agg_in_stripe(struct ec_agg_entry *entry, daos_recx_t *recx)
{
	unsigned int		len = entry->ae_oca->u.ec.e_len;
	unsigned int		k = entry->ae_oca->u.ec.e_k;
	daos_off_t		stripe = recx->rx_idx / (len * k);
	daos_off_t		stripe_end = (stripe + 1) * len * k;

	if (recx->rx_idx + recx->rx_nr > stripe_end)
		return stripe_end - recx->rx_idx;
	else
		return recx->rx_nr;
}

/* Iterator call back sub-function for handling data extents.
 */
static int
agg_data_extent(vos_iter_entry_t *entry, struct ec_agg_entry *agg_entry,
		daos_handle_t ih, unsigned int *acts)
{
	struct ec_agg_extent	*extent = NULL;
	int			 rc = 0;

	D_PRINT("Handling data extent: %lu,%lu, for stripe  %lu, shard: %u\n",
		extent->ae_recx.rx_idx, extent->ae_recx.rx_nr,
		agg_stripenum(agg_entry, extent->ae_recx.rx_idx),
		agg_entry->ae_oid.id_shard);
	if (agg_stripenum(agg_entry, entry->ie_recx.rx_idx) !=
			agg_entry->ae_cur_stripe.as_stripenum) {
		if (agg_entry->ae_cur_stripe.as_stripenum != ~0UL)
			agg_process_stripe(agg_entry);
		agg_entry->ae_cur_stripe.as_stripenum =
			agg_stripenum(agg_entry, entry->ie_recx.rx_idx);
	}
	D_ALLOC_PTR(extent);
	if (extent == NULL) {
		rc = -DER_NOMEM;
		goto out;
	}
	extent->ae_recx = entry->ie_recx;
	extent->ae_epoch = entry->ie_epoch;
	agg_entry->ae_rsize = entry->ie_rsize;

	d_list_add_tail(&extent->ae_link,
			&agg_entry->ae_cur_stripe.as_dextents);
	if (!agg_entry->ae_cur_stripe.as_extent_cnt)
		/* first extent in stripe: save the start offset */
		agg_entry->ae_cur_stripe.as_offset =  extent->ae_recx.rx_idx -
			agg_entry->ae_cur_stripe.as_stripenum *
			agg_entry->ae_oca->u.ec.e_len *
			agg_entry->ae_oca->u.ec.e_k;
	agg_entry->ae_cur_stripe.as_extent_cnt++;
	agg_entry->ae_cur_stripe.as_stripe_fill +=
		agg_in_stripe(agg_entry, &entry->ie_recx);

	if (extent->ae_epoch > agg_entry->ae_cur_stripe.as_hi_epoch)
		agg_entry->ae_cur_stripe.as_hi_epoch = extent->ae_epoch;

	D_DEBUG(DB_TRACE, "adding extent %lu,%lu, to stripe  %lu, shard: %u\n",
		extent->ae_recx.rx_idx, extent->ae_recx.rx_nr,
		agg_stripenum(agg_entry, extent->ae_recx.rx_idx),
		agg_entry->ae_oid.id_shard);
	D_PRINT("adding extent %lu,%lu, to stripe  %lu, shard: %u\n",
		extent->ae_recx.rx_idx, extent->ae_recx.rx_nr,
		agg_stripenum(agg_entry, extent->ae_recx.rx_idx),
		agg_entry->ae_oid.id_shard);
out:
	return rc;
}

static int
agg_akey_post(daos_handle_t ih, vos_iter_entry_t *entry,
	 struct ec_agg_entry *agg_entry,
	 unsigned int *acts)
{
	int rc = 0;

	if (agg_entry->ae_cur_stripe.as_extent_cnt)
		rc = agg_process_stripe(agg_entry);

	return rc;
}

/* Handles each replica extent returned by the RECX iterator.
 */
static int
agg_ev(daos_handle_t ih, vos_iter_entry_t *entry,
	struct ec_agg_entry *agg_entry, unsigned int *acts)
{
	int			rc = 0;

	D_ASSERT(!(entry->ie_recx.rx_idx & PARITY_INDICATOR));

	rc = agg_data_extent(entry, agg_entry, ih, acts);

	return rc;
}

/* Pre-subtree iteration call back for per-object iterator
 */
static int
agg_iterate_pre_cb(daos_handle_t ih, vos_iter_entry_t *entry,
	vos_iter_type_t type, vos_iter_param_t *param,
	void *cb_arg, unsigned int *acts)
{
	struct ec_agg_entry	*agg_entry = (struct ec_agg_entry *) cb_arg;
	int			 rc = 0;

	D_PRINT("lo: %lu, hi: %lu\n", agg_entry->ae_epr.epr_lo,
		agg_entry->ae_epr.epr_lo);

	switch (type) {
	case VOS_ITER_DKEY:
		rc = agg_dkey(ih, entry, agg_entry, acts);
		break;
	case VOS_ITER_AKEY:
		rc = agg_akey(ih, entry, agg_entry, acts);
		break;
	case VOS_ITER_RECX:
		rc = agg_ev(ih, entry, agg_entry, acts);
		break;
	default:
		break;
	}

	return rc;
}

/* Post iteration call back for per-object iterator
 */
static int
agg_iterate_post_cb(daos_handle_t ih, vos_iter_entry_t *entry,
	vos_iter_type_t type, vos_iter_param_t *param,
	void *cb_arg, unsigned int *acts)
{
	struct ec_agg_entry	*agg_entry = (struct ec_agg_entry *) cb_arg;
	int			 rc = 0;

	switch (type) {
	case VOS_ITER_DKEY:
		break;
	case VOS_ITER_AKEY:
		rc = agg_akey_post(ih, entry, agg_entry, acts);
		break;
	case VOS_ITER_RECX:
		break;
	default:
		break;
	}

	return rc;
}

/* Initializes the struct holding the iteration state (ec_agg_entry).
 */
static void
agg_reset_entry(daos_handle_t lcoh, daos_handle_t gcoh,
	       struct ec_agg_entry *agg_entry,
	       vos_iter_entry_t *entry, struct daos_oclass_attr *oca,
	       d_sg_list_t *sgl)
{
	agg_entry->ae_oid		= entry->ie_oid;
	agg_entry->ae_oca		= oca;
	agg_entry->ae_codec		= NULL;
	agg_entry->ae_sgl		= sgl;
	agg_entry->ae_chdl		= lcoh;
	agg_entry->ae_cont_hdl		= gcoh;
	agg_entry->ae_rsize		= 0UL;
	agg_entry->ae_obj_hdl		= DAOS_HDL_INVAL;

	memset(&agg_entry->ae_dkey, 0, sizeof(agg_entry->ae_dkey));
	memset(&agg_entry->ae_akey, 0, sizeof(agg_entry->ae_akey));
	memset(&agg_entry->ae_par_extent, 0, sizeof(agg_entry->ae_par_extent));

	agg_entry->ae_cur_stripe.as_stripenum	= 0UL;
	agg_entry->ae_cur_stripe.as_hi_epoch	= 0UL;
	agg_entry->ae_cur_stripe.as_stripe_fill = 0UL;
	agg_entry->ae_cur_stripe.as_extent_cnt	= 0U;
}

/* Configures and invokes nested iterator. Called from full-VOS object
 * iteration.
 */
static int
agg_subtree_iterate(daos_handle_t ih, daos_unit_oid_t *oid,
		    struct ec_agg_param *agg_param)
{
	vos_iter_param_t	 iter_param = { 0 };
	struct vos_iter_anchors  anchors = { 0 };
	int			 rc = 0;

	iter_param.ip_hdl		= DAOS_HDL_INVAL;
	iter_param.ip_ih		= ih;
	iter_param.ip_flags		= VOS_IT_RECX_VISIBLE;
	iter_param.ip_oid		= *oid;
	iter_param.ip_epr.epr_lo	= 0ULL;
	iter_param.ip_epr.epr_hi	= DAOS_EPOCH_MAX;
	iter_param.ip_epc_expr		= VOS_IT_EPC_RR;
	iter_param.ip_flags		= VOS_IT_RECX_VISIBLE;
	iter_param.ip_recx.rx_idx	= 0ULL;
	iter_param.ip_recx.rx_nr	= ~PARITY_INDICATOR;

	rc = vos_iterate(&iter_param, VOS_ITER_DKEY, true, &anchors,
			 agg_iterate_pre_cb, agg_iterate_post_cb,
			 agg_param->ap_agg_entry, NULL);
	return rc;
}

/* Call-back function for full VOS iteration outer iterator.
 */
static int
agg_iter_obj_pre_cb(daos_handle_t ih, vos_iter_entry_t *entry,
	vos_iter_type_t type, vos_iter_param_t *param,
	void *cb_arg, unsigned int *acts)
{
	struct ec_agg_param	*agg_param = (struct ec_agg_param *) cb_arg;
	struct daos_oclass_attr *oca;
	int			 rc = 0;

	D_PRINT("Got an object\n");
	if (!daos_oclass_is_ec(entry->ie_oid.id_pub, &oca))
		return rc;

	rc = ds_pool_check_leader(agg_param->ap_pool_info.api_pool_uuid,
				  &entry->ie_oid,
				  agg_param->ap_pool_info.api_pool_version);
	D_PRINT("We're checking if we're the leader, rc == %d\n", rc);
	if (rc == 1) {
		if (agg_param->ap_agg_entry == NULL) {
			D_ALLOC_PTR(agg_param->ap_agg_entry);
			if (entry == NULL) {
				rc = -DER_NOMEM;
				goto out;
			}
			D_INIT_LIST_HEAD(&agg_param->ap_agg_entry->
					 ae_cur_stripe.as_dextents);
			/*
			agg_param->ap_agg_entry->ae_pool_info =
				&agg_param->ap_pool_info;
			*/
			agg_param->ap_agg_entry->ae_epr = param->ip_epr;

		}
		agg_reset_entry(agg_param->ap_cont_handle,
			       agg_param->ap_pool_info.api_cont_hdl,
			       agg_param->ap_agg_entry, entry, oca,
			       &agg_param->ap_sgl);
		rc = agg_subtree_iterate(ih, &entry->ie_oid, agg_param);
		if (rc)
			D_ERROR("Subtred iterate failed "DF_RC"\n", DP_RC(rc));
	} else if (rc < 0) {
		D_ERROR("ds_pool_check_leader failed "DF_RC"\n", DP_RC(rc));
		rc = 0;
	}
out:
	return rc;
}

/* Iterates entire VOS. Invokes nested iterator to recurse through trees
 * for all objects meeting the criteria: object is EC, and this target is
 * leader.
 */
static int
agg_iterate_all(struct ds_cont_child *cont, daos_epoch_range_t *epr)
{
	vos_iter_param_t	 iter_param = { 0 };
	struct vos_iter_anchors  anchors = { 0 };
	struct ec_agg_param	 agg_param = { 0 };
	int			 rc = 0;

	D_PRINT("Aggregate all\n");
	uuid_copy(agg_param.ap_pool_info.api_pool_uuid,
		  cont->sc_pool->spc_uuid);
	uuid_copy(agg_param.ap_pool_info.api_cont_uuid, cont->sc_uuid);

	agg_param.ap_pool_info.api_pool_version =
		cont->sc_pool->spc_pool->sp_map_version;
	agg_param.ap_pool_info.api_pool = cont->sc_pool->spc_pool;
	agg_param.ap_cont_handle	= cont->sc_hdl;

	iter_param.ip_hdl		= cont->sc_hdl;
	iter_param.ip_epr.epr_lo	= 0ULL;
	iter_param.ip_epr.epr_hi	= DAOS_EPOCH_MAX;
	rc = vos_iterate(&iter_param, VOS_ITER_OBJ, false, &anchors,
			 agg_iter_obj_pre_cb, NULL, &agg_param, NULL);
	agg_sgl_fini(&agg_param.ap_sgl);
	return rc;

}

/* Public API call. Invoked from aggregation ULT  (container/srv_target.c).
 * Call to committed transaction table driven scan will also be called from
 * this function.
 */
int
ds_obj_ec_aggregate(struct ds_cont_child *cont, daos_epoch_range_t *epr)
{
	int	rc = 0;

	rc = agg_iterate_all(cont, epr);

	return rc;
}

