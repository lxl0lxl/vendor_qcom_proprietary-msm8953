VPP_TARGET_USES_GPU := NO
VPP_TARGET_USES_HVX := NO
VPP_TARGET_USES_FRC := NO

SUPPORTED_PLATFORMS := msm8937 msm8952 msm8953 msm8996 msmcobalt
ifneq (,$(filter $(SUPPORTED_PLATFORMS),$(TARGET_BOARD_PLATFORM)))
LOCAL_BOARD_PLATFORM := $(TARGET_BOARD_PLATFORM)
else
LOCAL_BOARD_PLATFORM := default
endif

ifeq ($(LOCAL_BOARD_PLATFORM),msm8937)
VPP_TARGET_USES_GPU := YES
LOCAL_CFLAGS += -DVPP_EXTERNAL_EXTRADATA_TYPE=EXTRADATA_BUFFER_FORMAT_OMX
else ifeq ($(LOCAL_BOARD_PLATFORM),msm8952)
VPP_TARGET_USES_GPU := YES
LOCAL_CFLAGS += -DVPP_EXTERNAL_EXTRADATA_TYPE=EXTRADATA_BUFFER_FORMAT_OMX
else ifeq ($(LOCAL_BOARD_PLATFORM),msm8953)
VPP_TARGET_USES_GPU := YES
LOCAL_CFLAGS += -DVPP_EXTERNAL_EXTRADATA_TYPE=EXTRADATA_BUFFER_FORMAT_OMX
else ifeq ($(LOCAL_BOARD_PLATFORM),msm8996)
VPP_TARGET_USES_HVX := YES
VPP_TARGET_USES_FRC := YES
VPP_TARGET_USES_HVX_CORE := YES
LOCAL_CFLAGS += -DVPP_EXTERNAL_EXTRADATA_TYPE=EXTRADATA_BUFFER_FORMAT_OMX
LOCAL_CFLAGS += -DVPP_RES_DI_IS_DISABLED_FOR_AUTO=VPP_RESOLUTION_UHD
else ifeq ($(LOCAL_BOARD_PLATFORM),msmcobalt)
VPP_TARGET_USES_HVX := YES
VPP_TARGET_USES_FRC := YES
VPP_TARGET_USES_HVX_CORE := YES
LOCAL_CFLAGS += -DVPP_EXTERNAL_EXTRADATA_TYPE=EXTRADATA_BUFFER_FORMAT_OMX
LOCAL_CFLAGS += -DVPP_RES_DI_IS_DISABLED_FOR_AUTO=VPP_RESOLUTION_UHD
endif

ifeq "$(VPP_TARGET_USES_GPU)" "YES"
LOCAL_CFLAGS += -DVPP_TARGET_USES_GPU=1
LOCAL_CFLAGS += -DLOCAL_BOARD_PLATFORM=\"$(LOCAL_BOARD_PLATFORM)\"
#$(info VPP_TARGET_USES_GPU=YES)
endif

ifeq "$(VPP_TARGET_USES_HVX)" "YES"
LOCAL_CFLAGS += -DVPP_TARGET_USES_HVX=1
#$(info VPP_TARGET_USES_HVX=YES)
endif

ifeq "$(VPP_TARGET_USES_FRC)" "YES"
LOCAL_CFLAGS += -DVPP_TARGET_USES_FRC=1
#$(info VPP_TARGET_USES_FRC=YES)
endif

ifeq "$(VPP_TEST_ENV)" "YES"
LOCAL_CFLAGS += -DVPP_TEST_BUILD
endif
