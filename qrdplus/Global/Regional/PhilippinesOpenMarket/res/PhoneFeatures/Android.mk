LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := $(call all-subdir-java-files)
LOCAL_SDK_VERSION := current
LOCAL_CERTIFICATE := shared

LOCAL_PACKAGE_NAME := PhilippinesOpenMarketPhoneFeaturesRes
LOCAL_MODULE_PATH := $(TARGET_OUT)/vendor/PhilippinesOpenMarket/system/vendor/overlay

include $(BUILD_PACKAGE)
