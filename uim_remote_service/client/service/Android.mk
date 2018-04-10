#/******************************************************************************
#*@file Android.mk
#*brief Rules for copiling the source files
#*******************************************************************************/
LOCAL_PATH:= $(call my-dir)

# Build the auto generated files into a library to be used by both the
# app and the service
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := debug
LOCAL_MODULE := uimremoteclientlibrary
LOCAL_SRC_FILES := src/com/qualcomm/uimremoteclient/IUimRemoteClientService.aidl
LOCAL_SRC_FILES += src/com/qualcomm/uimremoteclient/IUimRemoteClientServiceCallback.aidl

include $(BUILD_JAVA_LIBRARY)

# ==========================================================================
# Build the service
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := debug
src_proto := src
LOCAL_SRC_FILES := $(call all-subdir-java-files)
LOCAL_SRC_FILES += $(call all-proto-files-under, $(src_proto))

LOCAL_PROTOC_OPTIMIZE_TYPE := micro

LOCAL_PACKAGE_NAME := uimremoteclient
LOCAL_CERTIFICATE := platform
LOCAL_PROGUARD_ENABLED := disabled

LOCAL_JAVA_LIBRARIES := uimremoteclientlibrary

include $(BUILD_PACKAGE)

# ==========================================================================
include $(CLEAR_VARS)

LOCAL_MODULE := uimremoteclient.xml

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE_CLASS := ETC

# This will install the file in /system/etc/permissions
#
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/permissions

LOCAL_SRC_FILES := $(LOCAL_MODULE)

include $(BUILD_PREBUILT)
