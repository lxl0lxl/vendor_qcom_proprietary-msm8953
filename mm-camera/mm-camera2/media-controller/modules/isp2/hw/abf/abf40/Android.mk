#======================================================================
#makefile for libmmcamera_isp_abf40.so for mm-camera2
#======================================================================
ifeq ($(call is-vendor-board-platform,QCOM),true)
ifeq ($(call is-board-platform-in-list, msm8916 msm8974 msm8952 msm8937 msm8953),true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDFLAGS := $(mmcamera_debug_lflags)

LOCAL_CFLAGS :=  -DAMSS_VERSION=$(AMSS_VERSION) \
  $(mmcamera_debug_defines) \
  $(mmcamera_debug_cflags)

ifneq (,$(findstring $(PLATFORM_VERSION), 4.4 4.4.1 4.4.2))
LOCAL_CFLAGS  += -Werror
endif

LOCAL_MMCAMERA_PATH := $(LOCAL_PATH)/../../../../../../

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/includes/

LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/server-tuning/tuning/

LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/includes/

LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/bus/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/controller/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/event/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/module/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/object/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/pipeline/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/port/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/stream/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/tools/

LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/includes/
LOCAL_C_INCLUDES += $(LOCAL_CHROMATIX_PATH)

LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/isp2/common/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/isp2/hw/sub_module

ifeq ($(TARGET_COMPILE_WITH_MSM_KERNEL),true)
LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
endif
LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/include/mm-camera-interface
ifeq ($(call is-board-platform-in-list, msm8916 msm8974),true)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include40
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/isp2/module/isp40
else ifeq ($(call is-board-platform-in-list, msm8952),true)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include40
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/isp2/module/isp41
else ifeq ($(call is-board-platform-in-list, msm8937 msm8953),true)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include40
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/isp2/module/isp42
endif
ifeq ($(TARGET_COMPILE_WITH_MSM_KERNEL),true)
LOCAL_ADDITIONAL_DEPENDENCIES := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr
endif

LOCAL_SRC_FILES        := module_abf40.c abf40.c
ifeq ($(OEM_CHROMATIX), true)
LOCAL_SRC_FILES        += abf40_ext.c
LOCAL_CFLAGS += -DOVERRIDE_FUNC=1
endif
LOCAL_MODULE           := libmmcamera_isp_abf40
LOCAL_SHARED_LIBRARIES := libcutils \
                          libmmcamera2_mct \
                          libmmcamera_isp_sub_module \
                          libmmcamera_dbg

ifeq ($(MM_DEBUG),true)
LOCAL_SHARED_LIBRARIES += liblog
endif

LOCAL_MODULE_OWNER := qti
LOCAL_PROPRIETARY_MODULE := true

ifeq ($(32_BIT_FLAG), true)
LOCAL_32_BIT_ONLY := true
endif

include $(BUILD_SHARED_LIBRARY)

endif # if 8916
endif # is-vendor-board-platform,QCOM
