LOCAL_PATH := $(call my-dir)

$(shell mkdir -p $(TARGET_OUT)/vendor/iMobile/system/media)
$(shell cp -r $(LOCAL_PATH)/*.wav $(TARGET_OUT)/vendor/iMobile/system/media)
