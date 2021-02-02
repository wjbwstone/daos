// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: DaosObjectAttribute.proto

package io.daos.obj.attr;

/**
 * Protobuf type {@code objattr.DaosObjectAttribute}
 */
public final class DaosObjectAttribute extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:objattr.DaosObjectAttribute)
    DaosObjectAttributeOrBuilder {
private static final long serialVersionUID = 0L;
  // Use DaosObjectAttribute.newBuilder() to construct.
  private DaosObjectAttribute(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private DaosObjectAttribute() {
  }

  @Override
  @SuppressWarnings({"unused"})
  protected Object newInstance(
      UnusedPrivateParameter unused) {
    return new DaosObjectAttribute();
  }

  @Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private DaosObjectAttribute(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    this();
    if (extensionRegistry == null) {
      throw new NullPointerException();
    }
    com.google.protobuf.UnknownFieldSet.Builder unknownFields =
        com.google.protobuf.UnknownFieldSet.newBuilder();
    try {
      boolean done = false;
      while (!done) {
        int tag = input.readTag();
        switch (tag) {
          case 0:
            done = true;
            break;
          case 8: {

            oaRank_ = input.readUInt32();
            break;
          }
          case 18: {
            DaosOclassAttr.Builder subBuilder = null;
            if (oaOa_ != null) {
              subBuilder = oaOa_.toBuilder();
            }
            oaOa_ = input.readMessage(DaosOclassAttr.parser(), extensionRegistry);
            if (subBuilder != null) {
              subBuilder.mergeFrom(oaOa_);
              oaOa_ = subBuilder.buildPartial();
            }

            break;
          }
          default: {
            if (!parseUnknownField(
                input, unknownFields, extensionRegistry, tag)) {
              done = true;
            }
            break;
          }
        }
      }
    } catch (com.google.protobuf.InvalidProtocolBufferException e) {
      throw e.setUnfinishedMessage(this);
    } catch (java.io.IOException e) {
      throw new com.google.protobuf.InvalidProtocolBufferException(
          e).setUnfinishedMessage(this);
    } finally {
      this.unknownFields = unknownFields.build();
      makeExtensionsImmutable();
    }
  }
  public static final com.google.protobuf.Descriptors.Descriptor
      getDescriptor() {
    return DaosObjAttrClasses.internal_static_objattr_DaosObjectAttribute_descriptor;
  }

  @Override
  protected FieldAccessorTable
      internalGetFieldAccessorTable() {
    return DaosObjAttrClasses.internal_static_objattr_DaosObjectAttribute_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            DaosObjectAttribute.class, Builder.class);
  }

  public static final int OA_RANK_FIELD_NUMBER = 1;
  private int oaRank_;
  /**
   * <code>uint32 oa_rank = 1;</code>
   * @return The oaRank.
   */
  @Override
  public int getOaRank() {
    return oaRank_;
  }

  public static final int OA_OA_FIELD_NUMBER = 2;
  private DaosOclassAttr oaOa_;
  /**
   * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
   * @return Whether the oaOa field is set.
   */
  @Override
  public boolean hasOaOa() {
    return oaOa_ != null;
  }
  /**
   * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
   * @return The oaOa.
   */
  @Override
  public DaosOclassAttr getOaOa() {
    return oaOa_ == null ? DaosOclassAttr.getDefaultInstance() : oaOa_;
  }
  /**
   * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
   */
  @Override
  public DaosOclassAttrOrBuilder getOaOaOrBuilder() {
    return getOaOa();
  }

  private byte memoizedIsInitialized = -1;
  @Override
  public final boolean isInitialized() {
    byte isInitialized = memoizedIsInitialized;
    if (isInitialized == 1) return true;
    if (isInitialized == 0) return false;

    memoizedIsInitialized = 1;
    return true;
  }

  @Override
  public void writeTo(com.google.protobuf.CodedOutputStream output)
                      throws java.io.IOException {
    if (oaRank_ != 0) {
      output.writeUInt32(1, oaRank_);
    }
    if (oaOa_ != null) {
      output.writeMessage(2, getOaOa());
    }
    unknownFields.writeTo(output);
  }

  @Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (oaRank_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeUInt32Size(1, oaRank_);
    }
    if (oaOa_ != null) {
      size += com.google.protobuf.CodedOutputStream
        .computeMessageSize(2, getOaOa());
    }
    size += unknownFields.getSerializedSize();
    memoizedSize = size;
    return size;
  }

  @Override
  public boolean equals(final Object obj) {
    if (obj == this) {
     return true;
    }
    if (!(obj instanceof DaosObjectAttribute)) {
      return super.equals(obj);
    }
    DaosObjectAttribute other = (DaosObjectAttribute) obj;

    if (getOaRank()
        != other.getOaRank()) return false;
    if (hasOaOa() != other.hasOaOa()) return false;
    if (hasOaOa()) {
      if (!getOaOa()
          .equals(other.getOaOa())) return false;
    }
    if (!unknownFields.equals(other.unknownFields)) return false;
    return true;
  }

  @Override
  public int hashCode() {
    if (memoizedHashCode != 0) {
      return memoizedHashCode;
    }
    int hash = 41;
    hash = (19 * hash) + getDescriptor().hashCode();
    hash = (37 * hash) + OA_RANK_FIELD_NUMBER;
    hash = (53 * hash) + getOaRank();
    if (hasOaOa()) {
      hash = (37 * hash) + OA_OA_FIELD_NUMBER;
      hash = (53 * hash) + getOaOa().hashCode();
    }
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static DaosObjectAttribute parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static DaosObjectAttribute parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static DaosObjectAttribute parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static DaosObjectAttribute parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static DaosObjectAttribute parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static DaosObjectAttribute parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static DaosObjectAttribute parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static DaosObjectAttribute parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static DaosObjectAttribute parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static DaosObjectAttribute parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static DaosObjectAttribute parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static DaosObjectAttribute parseFrom(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }

  @Override
  public Builder newBuilderForType() { return newBuilder(); }
  public static Builder newBuilder() {
    return DEFAULT_INSTANCE.toBuilder();
  }
  public static Builder newBuilder(DaosObjectAttribute prototype) {
    return DEFAULT_INSTANCE.toBuilder().mergeFrom(prototype);
  }
  @Override
  public Builder toBuilder() {
    return this == DEFAULT_INSTANCE
        ? new Builder() : new Builder().mergeFrom(this);
  }

  @Override
  protected Builder newBuilderForType(
      BuilderParent parent) {
    Builder builder = new Builder(parent);
    return builder;
  }
  /**
   * Protobuf type {@code objattr.DaosObjectAttribute}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:objattr.DaosObjectAttribute)
      DaosObjectAttributeOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return DaosObjAttrClasses.internal_static_objattr_DaosObjectAttribute_descriptor;
    }

    @Override
    protected FieldAccessorTable
        internalGetFieldAccessorTable() {
      return DaosObjAttrClasses.internal_static_objattr_DaosObjectAttribute_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              DaosObjectAttribute.class, Builder.class);
    }

    // Construct using io.daos.obj.attr.DaosObjectAttribute.newBuilder()
    private Builder() {
      maybeForceBuilderInitialization();
    }

    private Builder(
        BuilderParent parent) {
      super(parent);
      maybeForceBuilderInitialization();
    }
    private void maybeForceBuilderInitialization() {
      if (com.google.protobuf.GeneratedMessageV3
              .alwaysUseFieldBuilders) {
      }
    }
    @Override
    public Builder clear() {
      super.clear();
      oaRank_ = 0;

      if (oaOaBuilder_ == null) {
        oaOa_ = null;
      } else {
        oaOa_ = null;
        oaOaBuilder_ = null;
      }
      return this;
    }

    @Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return DaosObjAttrClasses.internal_static_objattr_DaosObjectAttribute_descriptor;
    }

    @Override
    public DaosObjectAttribute getDefaultInstanceForType() {
      return DaosObjectAttribute.getDefaultInstance();
    }

    @Override
    public DaosObjectAttribute build() {
      DaosObjectAttribute result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @Override
    public DaosObjectAttribute buildPartial() {
      DaosObjectAttribute result = new DaosObjectAttribute(this);
      result.oaRank_ = oaRank_;
      if (oaOaBuilder_ == null) {
        result.oaOa_ = oaOa_;
      } else {
        result.oaOa_ = oaOaBuilder_.build();
      }
      onBuilt();
      return result;
    }

    @Override
    public Builder clone() {
      return super.clone();
    }
    @Override
    public Builder setField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        Object value) {
      return super.setField(field, value);
    }
    @Override
    public Builder clearField(
        com.google.protobuf.Descriptors.FieldDescriptor field) {
      return super.clearField(field);
    }
    @Override
    public Builder clearOneof(
        com.google.protobuf.Descriptors.OneofDescriptor oneof) {
      return super.clearOneof(oneof);
    }
    @Override
    public Builder setRepeatedField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        int index, Object value) {
      return super.setRepeatedField(field, index, value);
    }
    @Override
    public Builder addRepeatedField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        Object value) {
      return super.addRepeatedField(field, value);
    }
    @Override
    public Builder mergeFrom(com.google.protobuf.Message other) {
      if (other instanceof DaosObjectAttribute) {
        return mergeFrom((DaosObjectAttribute)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(DaosObjectAttribute other) {
      if (other == DaosObjectAttribute.getDefaultInstance()) return this;
      if (other.getOaRank() != 0) {
        setOaRank(other.getOaRank());
      }
      if (other.hasOaOa()) {
        mergeOaOa(other.getOaOa());
      }
      this.mergeUnknownFields(other.unknownFields);
      onChanged();
      return this;
    }

    @Override
    public final boolean isInitialized() {
      return true;
    }

    @Override
    public Builder mergeFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      DaosObjectAttribute parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (DaosObjectAttribute) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private int oaRank_ ;
    /**
     * <code>uint32 oa_rank = 1;</code>
     * @return The oaRank.
     */
    @Override
    public int getOaRank() {
      return oaRank_;
    }
    /**
     * <code>uint32 oa_rank = 1;</code>
     * @param value The oaRank to set.
     * @return This builder for chaining.
     */
    public Builder setOaRank(int value) {

      oaRank_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>uint32 oa_rank = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearOaRank() {

      oaRank_ = 0;
      onChanged();
      return this;
    }

    private DaosOclassAttr oaOa_;
    private com.google.protobuf.SingleFieldBuilderV3<
        DaosOclassAttr, DaosOclassAttr.Builder, DaosOclassAttrOrBuilder> oaOaBuilder_;
    /**
     * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
     * @return Whether the oaOa field is set.
     */
    public boolean hasOaOa() {
      return oaOaBuilder_ != null || oaOa_ != null;
    }
    /**
     * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
     * @return The oaOa.
     */
    public DaosOclassAttr getOaOa() {
      if (oaOaBuilder_ == null) {
        return oaOa_ == null ? DaosOclassAttr.getDefaultInstance() : oaOa_;
      } else {
        return oaOaBuilder_.getMessage();
      }
    }
    /**
     * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
     */
    public Builder setOaOa(DaosOclassAttr value) {
      if (oaOaBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        oaOa_ = value;
        onChanged();
      } else {
        oaOaBuilder_.setMessage(value);
      }

      return this;
    }
    /**
     * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
     */
    public Builder setOaOa(
        DaosOclassAttr.Builder builderForValue) {
      if (oaOaBuilder_ == null) {
        oaOa_ = builderForValue.build();
        onChanged();
      } else {
        oaOaBuilder_.setMessage(builderForValue.build());
      }

      return this;
    }
    /**
     * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
     */
    public Builder mergeOaOa(DaosOclassAttr value) {
      if (oaOaBuilder_ == null) {
        if (oaOa_ != null) {
          oaOa_ =
            DaosOclassAttr.newBuilder(oaOa_).mergeFrom(value).buildPartial();
        } else {
          oaOa_ = value;
        }
        onChanged();
      } else {
        oaOaBuilder_.mergeFrom(value);
      }

      return this;
    }
    /**
     * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
     */
    public Builder clearOaOa() {
      if (oaOaBuilder_ == null) {
        oaOa_ = null;
        onChanged();
      } else {
        oaOa_ = null;
        oaOaBuilder_ = null;
      }

      return this;
    }
    /**
     * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
     */
    public DaosOclassAttr.Builder getOaOaBuilder() {

      onChanged();
      return getOaOaFieldBuilder().getBuilder();
    }
    /**
     * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
     */
    public DaosOclassAttrOrBuilder getOaOaOrBuilder() {
      if (oaOaBuilder_ != null) {
        return oaOaBuilder_.getMessageOrBuilder();
      } else {
        return oaOa_ == null ?
            DaosOclassAttr.getDefaultInstance() : oaOa_;
      }
    }
    /**
     * <code>.objattr.DaosOclassAttr oa_oa = 2;</code>
     */
    private com.google.protobuf.SingleFieldBuilderV3<
        DaosOclassAttr, DaosOclassAttr.Builder, DaosOclassAttrOrBuilder>
        getOaOaFieldBuilder() {
      if (oaOaBuilder_ == null) {
        oaOaBuilder_ = new com.google.protobuf.SingleFieldBuilderV3<
            DaosOclassAttr, DaosOclassAttr.Builder, DaosOclassAttrOrBuilder>(
                getOaOa(),
                getParentForChildren(),
                isClean());
        oaOa_ = null;
      }
      return oaOaBuilder_;
    }
    @Override
    public final Builder setUnknownFields(
        final com.google.protobuf.UnknownFieldSet unknownFields) {
      return super.setUnknownFields(unknownFields);
    }

    @Override
    public final Builder mergeUnknownFields(
        final com.google.protobuf.UnknownFieldSet unknownFields) {
      return super.mergeUnknownFields(unknownFields);
    }


    // @@protoc_insertion_point(builder_scope:objattr.DaosObjectAttribute)
  }

  // @@protoc_insertion_point(class_scope:objattr.DaosObjectAttribute)
  private static final DaosObjectAttribute DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new DaosObjectAttribute();
  }

  public static DaosObjectAttribute getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<DaosObjectAttribute>
      PARSER = new com.google.protobuf.AbstractParser<DaosObjectAttribute>() {
    @Override
    public DaosObjectAttribute parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new DaosObjectAttribute(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<DaosObjectAttribute> parser() {
    return PARSER;
  }

  @Override
  public com.google.protobuf.Parser<DaosObjectAttribute> getParserForType() {
    return PARSER;
  }

  @Override
  public DaosObjectAttribute getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

