LOCAL_PATH := $(call my-dir)

#----------------------------------------------------------------------
# Copy additional target-specific files
#----------------------------------------------------------------------
include $(CLEAR_VARS)
LOCAL_MODULE       := init.target.rc
LOCAL_MODULE_TAGS  := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES    := $(LOCAL_MODULE)
LOCAL_MODULE_PATH  := $(TARGET_OUT_VENDOR_ETC)/init/hw
include $(BUILD_PREBUILT)

#----------------------------------------------------------------------
# Configs common to AndroidBoard.mk for all targets
#----------------------------------------------------------------------
include vendor/qcom/opensource/core-utils/build/AndroidBoardCommon.mk