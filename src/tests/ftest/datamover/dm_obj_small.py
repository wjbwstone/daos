#!/usr/bin/python
'''
  (C) Copyright 2020-2021 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
'''
from data_mover_test_base import DataMoverTestBase
from pydaos.raw import IORequest, DaosObj, DaosApiError
import ctypes
import avocado
import time


class DmObjSmall(DataMoverTestBase):
    # pylint: disable=too-many-ancestors
    """Object Data Mover validation for syncing/cloning generic containers
       at the object level.

    Test Class Description:
        Tests the following cases:
            Cloning a small container with dcp.
    :avocado: recursive
    """

    # TODO remove this
    def tearDown(self):
        pass

    def run_dm_obj_small(self, tool):
        """
        Test Description:
            Tests cloning a small container.
        Use Cases:
            Create pool1.
            Create cont1 in pool1.
            Create a small dataset in cont1.
            Clone cont1 to a new cont2 in pool1.
            Create pool2.
            Clone cont1 to a new cont3 in pool2.
        """
        # Set the tool to use
        self.set_tool(tool)

        # Create pool1
        pool1 = self.create_pool()
        pool1.connect(2)

        # Create cont1
        cont1 = self.create_cont(pool1)
        cont1.open()

        # Get the dataset params
        num_objs = self.params.get(
            "num_objs", "/run/dataset/*")
        num_dkeys_per_obj = self.params.get(
            "num_dkeys_per_obj", "/run/dataset/*")
        num_akeys_array_per_dkey = self.params.get(
            "num_akeys_array_per_dkey", "/run/dataset/*")
        num_akeys_single_per_dkey = self.params.get(
            "num_akeys_single_per_dkey", "/run/dataset/*")
        akey_sizes = self.params.get(
            "akey_sizes", "/run/dataset/*")
        akey_extents = self.params.get(
            "akey_extents", "/run/dataset/*")

        total_num_extents = 0
        # Create a small dataset in cont1.
        obj_list = []
        self.log.info("Creating dataset in %s/%s",
            str(pool1.uuid), str(cont1.uuid))
        for obj_idx in range(num_objs):
            # Create a new obj
            obj = DaosObj(pool1.context, cont1.container)
            obj_list.append(obj)

            obj.create(rank=obj_idx, objcls=2)
            obj.open()
            self.log.info("context = {}".format(pool1.context))
            ioreq = IORequest(pool1.context, cont1.container, obj)
            for dkey_idx in range(num_dkeys_per_obj):
                c_dkey = ctypes.create_string_buffer(
                    "dkey {}".format(dkey_idx))

                for akey_idx in range(num_akeys_single_per_dkey):
                    # Round-robin to get the size of data and
                    # arbitrarily use a number 0-9 to fill data
                    data_size = akey_sizes[akey_idx % len(akey_sizes)]
                    data_val = chr((akey_idx % 10) + 65)
                    data = data_size * data_val
                    c_akey = ctypes.create_string_buffer(
                        "akey single {}".format(akey_idx))
                    c_value = ctypes.create_string_buffer(data)
                    c_size = ctypes.c_size_t(ctypes.sizeof(c_value))
                    ioreq.single_insert(c_dkey, c_akey, c_value, c_size)
                    total_num_extents += 1

                for akey_idx in range(num_akeys_array_per_dkey):
                    # Round-robin to get the size of data and
                    # the number of extents, and
                    # arbitrarily use a number 0-9 to fill data
                    size_idx = akey_idx % len(akey_sizes)
                    data_size = akey_sizes[size_idx]
                    num_extents = akey_extents[size_idx]
                    total_num_extents += num_extents
                    c_data = []
                    c_akey = ctypes.create_string_buffer(
                        "akey array {}".format(akey_idx))
                    for data_idx in range(num_extents):
                        data_val = chr((data_idx % 10) + 65)
                        data = data_size * data_val
                        c_data.append([ctypes.create_string_buffer(data), data_size])
                    ioreq.insert_array(c_dkey, c_akey, c_data)
            obj.close()

        # Generate a uuid for cont2
        cont2_uuid = self.gen_uuid()

        # Clone cont1 to a new cont2 in pool1
        # TODO replace " " after rebasing on the dysnc PR
        self.run_datamover(
            self.test_id + " (cont1->cont2) (same pool)",
            "DAOS_UUID", " ", pool1, cont1,
            "DAOS_UUID", " ", pool1, cont2_uuid)

        # Verify data in cont2
        cont2 = self.get_cont(pool1, cont2_uuid)
        cont2.open()
        self.log.info("total_num_extents = {}".format(total_num_extents))
        self.log.info("pool1={}\ncont1={}".format(str(pool1.uuid), str(cont1.uuid)))
        self.log.info("cont2={}".format(str(cont2.uuid)))
        self.log.info("Verifying dataset in %s/%s",
            str(pool1.uuid), str(cont2.uuid))
        for obj_idx in range(num_objs):
            # Open the object in cont2
            obj = DaosObj(pool1.context, cont2.container, obj_list[obj_idx].c_oid)
            obj.open()
            ioreq = IORequest(pool1.context, cont2.container, obj)
            for dkey_idx in range(num_dkeys_per_obj):
                dkey = "dkey {}".format(dkey_idx)
                c_dkey = ctypes.create_string_buffer(dkey)

                for akey_idx in range(num_akeys_single_per_dkey):
                    # Round-robin to get the size of data and
                    # arbitrarily use a number 0-9 to fill data
                    data_size = akey_sizes[akey_idx % len(akey_sizes)]
                    data_val = chr((akey_idx % 10) + 65)
                    data = data_size * data_val
                    akey = "akey single {}".format(akey_idx)
                    c_akey = ctypes.create_string_buffer(akey)
                    buf = ioreq.single_fetch(c_dkey, c_akey, data_size + 1)
                    actual_data = str(buf.value)
                    if actual_data != data:
                        self.log.info("Got\n{}\nbut expected\n{}".format(actual_data, data))
                        self.fail("Incorrect data for obj {}, {}, {}".format(
                            str(obj_idx), dkey, akey))

                for akey_idx in range(num_akeys_array_per_dkey):
                    # Round-robin to get the size of data and
                    # the number of extents, and
                    # arbitrarily use a number 0-9 to fill data
                    size_idx = akey_idx % len(akey_sizes)
                    data_size = akey_sizes[size_idx]
                    c_data_size = c_rec_size = ctypes.c_size_t(data_size)
                    num_extents = akey_extents[size_idx]
                    c_num_extents = ctypes.c_uint(num_extents)
                    c_data = [[None, None]] * num_extents
                    c_akey = ctypes.create_string_buffer(
                        "akey array {}".format(akey_idx))
                    actual_data = ioreq.fetch_array(c_dkey, c_akey, c_num_extents, c_data_size)
                    for data_idx in range(num_extents):
                        data_val = chr((data_idx % 10) + 65)
                        data = data_size * data_val
                        if data != actual_data[data_idx]:
                            self.log.info("Got\n{}\nbut expected\n{}".format(actual_data[data_idx], data[data_idx]))
                            self.fail("Array compare failed")

            obj.close()
        cont2.close()

        self.fail("HERE")

        # Create pool2
        pool2 = self.create_pool()

        # Generate a uuid for cont3
        cont3_uuid = self.gen_uuid()

        # Clone cont1 to a new cont3 in pool2
        self.run_datamover(
            self.test_id + " (cont1->cont3) (different pool)",
            "DAOS_UUID", " ", pool1, cont1,
            "DAOS_UUID", " ", pool2, cont3_uuid)

        # Verify data in cont3
        cont3 = self.get_cont(pool2, cont3_uuid)
        cont3.open()
        # TODO fetch and compare data
        cont3.close()

    @avocado.fail_on(DaosApiError)
    def test_dm_obj_small_dcp(self):
        """
        Test Description:
            DAOS-6858: Verify cloning a small container with dcp.
        :avocado: tags=all,daily_regression
        :avocado: tags=datamover,dcp
        :avocado: tags=dm_obj_small,dm_obj_small_dcp
        """
        self.run_dm_obj_small("DCP")
