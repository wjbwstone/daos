/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: DaosObjectAttribute.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "DaosObjectAttribute.pb-c.h"
void   objattr__daos_rp_attr__init
                     (Objattr__DaosRpAttr         *message)
{
  static const Objattr__DaosRpAttr init_value = OBJATTR__DAOS_RP_ATTR__INIT;
  *message = init_value;
}
size_t objattr__daos_rp_attr__get_packed_size
                     (const Objattr__DaosRpAttr *message)
{
  assert(message->base.descriptor == &objattr__daos_rp_attr__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t objattr__daos_rp_attr__pack
                     (const Objattr__DaosRpAttr *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &objattr__daos_rp_attr__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t objattr__daos_rp_attr__pack_to_buffer
                     (const Objattr__DaosRpAttr *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &objattr__daos_rp_attr__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Objattr__DaosRpAttr *
       objattr__daos_rp_attr__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Objattr__DaosRpAttr *)
     protobuf_c_message_unpack (&objattr__daos_rp_attr__descriptor,
                                allocator, len, data);
}
void   objattr__daos_rp_attr__free_unpacked
                     (Objattr__DaosRpAttr *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &objattr__daos_rp_attr__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   objattr__daos_ec_attr__init
                     (Objattr__DaosEcAttr         *message)
{
  static const Objattr__DaosEcAttr init_value = OBJATTR__DAOS_EC_ATTR__INIT;
  *message = init_value;
}
size_t objattr__daos_ec_attr__get_packed_size
                     (const Objattr__DaosEcAttr *message)
{
  assert(message->base.descriptor == &objattr__daos_ec_attr__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t objattr__daos_ec_attr__pack
                     (const Objattr__DaosEcAttr *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &objattr__daos_ec_attr__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t objattr__daos_ec_attr__pack_to_buffer
                     (const Objattr__DaosEcAttr *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &objattr__daos_ec_attr__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Objattr__DaosEcAttr *
       objattr__daos_ec_attr__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Objattr__DaosEcAttr *)
     protobuf_c_message_unpack (&objattr__daos_ec_attr__descriptor,
                                allocator, len, data);
}
void   objattr__daos_ec_attr__free_unpacked
                     (Objattr__DaosEcAttr *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &objattr__daos_ec_attr__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   objattr__daos_oclass_attr__init
                     (Objattr__DaosOclassAttr         *message)
{
  static const Objattr__DaosOclassAttr init_value = OBJATTR__DAOS_OCLASS_ATTR__INIT;
  *message = init_value;
}
size_t objattr__daos_oclass_attr__get_packed_size
                     (const Objattr__DaosOclassAttr *message)
{
  assert(message->base.descriptor == &objattr__daos_oclass_attr__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t objattr__daos_oclass_attr__pack
                     (const Objattr__DaosOclassAttr *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &objattr__daos_oclass_attr__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t objattr__daos_oclass_attr__pack_to_buffer
                     (const Objattr__DaosOclassAttr *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &objattr__daos_oclass_attr__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Objattr__DaosOclassAttr *
       objattr__daos_oclass_attr__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Objattr__DaosOclassAttr *)
     protobuf_c_message_unpack (&objattr__daos_oclass_attr__descriptor,
                                allocator, len, data);
}
void   objattr__daos_oclass_attr__free_unpacked
                     (Objattr__DaosOclassAttr *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &objattr__daos_oclass_attr__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   objattr__daos_object_attribute__init
                     (Objattr__DaosObjectAttribute         *message)
{
  static const Objattr__DaosObjectAttribute init_value = OBJATTR__DAOS_OBJECT_ATTRIBUTE__INIT;
  *message = init_value;
}
size_t objattr__daos_object_attribute__get_packed_size
                     (const Objattr__DaosObjectAttribute *message)
{
  assert(message->base.descriptor == &objattr__daos_object_attribute__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t objattr__daos_object_attribute__pack
                     (const Objattr__DaosObjectAttribute *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &objattr__daos_object_attribute__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t objattr__daos_object_attribute__pack_to_buffer
                     (const Objattr__DaosObjectAttribute *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &objattr__daos_object_attribute__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Objattr__DaosObjectAttribute *
       objattr__daos_object_attribute__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Objattr__DaosObjectAttribute *)
     protobuf_c_message_unpack (&objattr__daos_object_attribute__descriptor,
                                allocator, len, data);
}
void   objattr__daos_object_attribute__free_unpacked
                     (Objattr__DaosObjectAttribute *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &objattr__daos_object_attribute__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor objattr__daos_rp_attr__field_descriptors[2] =
{
  {
    "r_proto",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosRpAttr, r_proto),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "r_num",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosRpAttr, r_num),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned objattr__daos_rp_attr__field_indices_by_name[] = {
  1,   /* field[1] = r_num */
  0,   /* field[0] = r_proto */
};
static const ProtobufCIntRange objattr__daos_rp_attr__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor objattr__daos_rp_attr__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "objattr.DaosRpAttr",
  "DaosRpAttr",
  "Objattr__DaosRpAttr",
  "objattr",
  sizeof(Objattr__DaosRpAttr),
  2,
  objattr__daos_rp_attr__field_descriptors,
  objattr__daos_rp_attr__field_indices_by_name,
  1,  objattr__daos_rp_attr__number_ranges,
  (ProtobufCMessageInit) objattr__daos_rp_attr__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor objattr__daos_ec_attr__field_descriptors[3] =
{
  {
    "e_k",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosEcAttr, e_k),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "e_p",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosEcAttr, e_p),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "e_len",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosEcAttr, e_len),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned objattr__daos_ec_attr__field_indices_by_name[] = {
  0,   /* field[0] = e_k */
  2,   /* field[2] = e_len */
  1,   /* field[1] = e_p */
};
static const ProtobufCIntRange objattr__daos_ec_attr__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor objattr__daos_ec_attr__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "objattr.DaosEcAttr",
  "DaosEcAttr",
  "Objattr__DaosEcAttr",
  "objattr",
  sizeof(Objattr__DaosEcAttr),
  3,
  objattr__daos_ec_attr__field_descriptors,
  objattr__daos_ec_attr__field_indices_by_name,
  1,  objattr__daos_ec_attr__number_ranges,
  (ProtobufCMessageInit) objattr__daos_ec_attr__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor objattr__daos_oclass_attr__field_descriptors[6] =
{
  {
    "ca_schema",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosOclassAttr, ca_schema),
    &objattr__daos_obj_schema__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "ca_resil",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosOclassAttr, ca_resil),
    &objattr__daos_obj_resil__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "ca_resil_degree",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosOclassAttr, ca_resil_degree),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "ca_grp_nr",
    4,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosOclassAttr, ca_grp_nr),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rp",
    5,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Objattr__DaosOclassAttr, u_case),
    offsetof(Objattr__DaosOclassAttr, rp),
    &objattr__daos_rp_attr__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "ec",
    6,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Objattr__DaosOclassAttr, u_case),
    offsetof(Objattr__DaosOclassAttr, ec),
    &objattr__daos_ec_attr__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned objattr__daos_oclass_attr__field_indices_by_name[] = {
  3,   /* field[3] = ca_grp_nr */
  1,   /* field[1] = ca_resil */
  2,   /* field[2] = ca_resil_degree */
  0,   /* field[0] = ca_schema */
  5,   /* field[5] = ec */
  4,   /* field[4] = rp */
};
static const ProtobufCIntRange objattr__daos_oclass_attr__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 6 }
};
const ProtobufCMessageDescriptor objattr__daos_oclass_attr__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "objattr.DaosOclassAttr",
  "DaosOclassAttr",
  "Objattr__DaosOclassAttr",
  "objattr",
  sizeof(Objattr__DaosOclassAttr),
  6,
  objattr__daos_oclass_attr__field_descriptors,
  objattr__daos_oclass_attr__field_indices_by_name,
  1,  objattr__daos_oclass_attr__number_ranges,
  (ProtobufCMessageInit) objattr__daos_oclass_attr__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor objattr__daos_object_attribute__field_descriptors[2] =
{
  {
    "oa_rank",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosObjectAttribute, oa_rank),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "oa_oa",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Objattr__DaosObjectAttribute, oa_oa),
    &objattr__daos_oclass_attr__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned objattr__daos_object_attribute__field_indices_by_name[] = {
  1,   /* field[1] = oa_oa */
  0,   /* field[0] = oa_rank */
};
static const ProtobufCIntRange objattr__daos_object_attribute__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor objattr__daos_object_attribute__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "objattr.DaosObjectAttribute",
  "DaosObjectAttribute",
  "Objattr__DaosObjectAttribute",
  "objattr",
  sizeof(Objattr__DaosObjectAttribute),
  2,
  objattr__daos_object_attribute__field_descriptors,
  objattr__daos_object_attribute__field_indices_by_name,
  1,  objattr__daos_object_attribute__number_ranges,
  (ProtobufCMessageInit) objattr__daos_object_attribute__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue objattr__daos_obj_schema__enum_values_by_number[4] =
{
  { "DAOS_OS_SINGLE", "OBJATTR__DAOS_OBJ_SCHEMA__DAOS_OS_SINGLE", 0 },
  { "DAOS_OS_STRIPED", "OBJATTR__DAOS_OBJ_SCHEMA__DAOS_OS_STRIPED", 1 },
  { "DAOS_OS_DYN_STRIPED", "OBJATTR__DAOS_OBJ_SCHEMA__DAOS_OS_DYN_STRIPED", 2 },
  { "DAOS_OS_DYN_CHUNKED", "OBJATTR__DAOS_OBJ_SCHEMA__DAOS_OS_DYN_CHUNKED", 3 },
};
static const ProtobufCIntRange objattr__daos_obj_schema__value_ranges[] = {
{0, 0},{0, 4}
};
static const ProtobufCEnumValueIndex objattr__daos_obj_schema__enum_values_by_name[4] =
{
  { "DAOS_OS_DYN_CHUNKED", 3 },
  { "DAOS_OS_DYN_STRIPED", 2 },
  { "DAOS_OS_SINGLE", 0 },
  { "DAOS_OS_STRIPED", 1 },
};
const ProtobufCEnumDescriptor objattr__daos_obj_schema__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "objattr.DaosObjSchema",
  "DaosObjSchema",
  "Objattr__DaosObjSchema",
  "objattr",
  4,
  objattr__daos_obj_schema__enum_values_by_number,
  4,
  objattr__daos_obj_schema__enum_values_by_name,
  1,
  objattr__daos_obj_schema__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCEnumValue objattr__daos_obj_resil__enum_values_by_number[2] =
{
  { "DAOS_RES_EC", "OBJATTR__DAOS_OBJ_RESIL__DAOS_RES_EC", 0 },
  { "DAOS_RES_REPL", "OBJATTR__DAOS_OBJ_RESIL__DAOS_RES_REPL", 1 },
};
static const ProtobufCIntRange objattr__daos_obj_resil__value_ranges[] = {
{0, 0},{0, 2}
};
static const ProtobufCEnumValueIndex objattr__daos_obj_resil__enum_values_by_name[2] =
{
  { "DAOS_RES_EC", 0 },
  { "DAOS_RES_REPL", 1 },
};
const ProtobufCEnumDescriptor objattr__daos_obj_resil__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "objattr.DaosObjResil",
  "DaosObjResil",
  "Objattr__DaosObjResil",
  "objattr",
  2,
  objattr__daos_obj_resil__enum_values_by_number,
  2,
  objattr__daos_obj_resil__enum_values_by_name,
  1,
  objattr__daos_obj_resil__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
