LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := binkaui
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/android/$(TARGET_ARCH_ABI)/libbinkaui.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := binkaengine
LOCAL_SRC_FILES := $(LOCAL_PATH)/lib/android/$(TARGET_ARCH_ABI)/libbinkaengine.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

include $(PREBUILT_SHARED_LIBRARY)
