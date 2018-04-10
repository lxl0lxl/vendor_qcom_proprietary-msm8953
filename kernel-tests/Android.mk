# Build only for qcom targets
KERNEL_TEST_PRODUCT_LIST := msm7625_surf
KERNEL_TEST_PRODUCT_LIST += msm7625_ffa
KERNEL_TEST_PRODUCT_LIST += msm7627_surf
KERNEL_TEST_PRODUCT_LIST += msm7627_6x
KERNEL_TEST_PRODUCT_LIST += msm7627_7x_surf
KERNEL_TEST_PRODUCT_LIST += msm7627_ffa
KERNEL_TEST_PRODUCT_LIST += msm7627a
KERNEL_TEST_PRODUCT_LIST += msm7630_surf
KERNEL_TEST_PRODUCT_LIST += msm7630_1x
KERNEL_TEST_PRODUCT_LIST += msm7630_fusion
KERNEL_TEST_PRODUCT_LIST += msm8660
KERNEL_TEST_PRODUCT_LIST += qsd8250_surf
KERNEL_TEST_PRODUCT_LIST += qsd8250_ffa
KERNEL_TEST_PRODUCT_LIST += qsd8650a_st1x
KERNEL_TEST_PRODUCT_LIST += msm8660_csfb
KERNEL_TEST_PRODUCT_LIST += msm8960
KERNEL_TEST_PRODUCT_LIST += msm8974
KERNEL_TEST_PRODUCT_LIST += msm8610
KERNEL_TEST_PRODUCT_LIST += msm8226
KERNEL_TEST_PRODUCT_LIST += apq8084
KERNEL_TEST_PRODUCT_LIST += msm_bronze
KERNEL_TEST_PRODUCT_LIST += msm8916
KERNEL_TEST_PRODUCT_LIST += msm8909
KERNEL_TEST_PRODUCT_LIST += msm8939
KERNEL_TEST_PRODUCT_LIST += msm8952
KERNEL_TEST_PRODUCT_LIST += mpq8092
KERNEL_TEST_PRODUCT_LIST += msm8992
KERNEL_TEST_PRODUCT_LIST += msm8994
KERNEL_TEST_PRODUCT_LIST += msm8996
KERNEL_TEST_PRODUCT_LIST += msm8976
KERNEL_TEST_PRODUCT_LIST += msm8956
KERNEL_TEST_PRODUCT_LIST += msm8952
KERNEL_TEST_PRODUCT_LIST += msm8953
KERNEL_TEST_PRODUCT_LIST += msm8937
KERNEL_TEST_PRODUCT_LIST += msmcobalt
KERNEL_TEST_PRODUCT_LIST += msmfalcon

ifeq ($(call is-board-platform-in-list,$(KERNEL_TEST_PRODUCT_LIST)),true)

# Use a private variable to save the result of $(call my-dir), because it will
# get squashed and will be irrecoverable later
this_folder := $(call my-dir)
include $(call all-subdir-makefiles)

LOCAL_PATH := $(this_folder)

include $(CLEAR_VARS)
LOCAL_MODULE := test_env_setup.sh
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_SRC_FILES := test_env_setup.sh
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_DATA)/kernel-tests
LOCAL_MODULE_OWNER := qcom
include $(BUILD_PREBUILT)

endif # KERNEL_TEST_PRODUCT_LIST
