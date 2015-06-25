LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos_protobuf_static
LOCAL_MODULE_FILENAME := libprotobuf-lite
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libprotobuf-lite.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)
