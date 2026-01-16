# config.mk
#
# Product-specific compile-time definitions.
#
# TODO(b/124534788): Temporarily allow eng and debug LOCAL_MODULE_TAGS

BUILD_BROKEN_CLANG_PROPERTY := true
BUILD_BROKEN_PREBUILT_ELF_FILES := true
BUILD_BROKEN_USES_BUILD_HOST_SHARED_LIBRARY := true
BUILD_BROKEN_USES_BUILD_HOST_STATIC_LIBRARY := true
BUILD_BROKEN_USES_BUILD_HOST_EXECUTABLE := true
BUILD_BROKEN_DUP_RULES := true
BUILD_BROKEN_MISSING_REQUIRED_MODULES  := true

BUILD_BROKEN_PLUGIN_VALIDATION := vndfuzztest

BOARD_PERSISTIMAGE_PARTITION_SIZE := 33554432
BOARD_PERSISTIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_USES_METADATA_PARTITION := true
BOARD_METADATAIMAGE_FILE_SYSTEM_TYPE := f2fs
BOARD_METADATAIMAGE_PARTITION_SIZE := 67108864
BOARD_DTBOIMG_PARTITION_SIZE := 0x1600000
SOONG_CONFIG_NAMESPACES += ufsbsg

SOONG_CONFIG_ufsbsg += ufsframework
SOONG_CONFIG_ufsbsg_ufsframework := bsg

#Enable PD locater/notifier
TARGET_PD_SERVICE_ENABLED := true

#Enable peripheral manager
TARGET_PER_MGR_ENABLED := true

ifeq ($(TARGET_USES_QMAA), true)
    ifneq ($(TARGET_USES_QMAA_OVERRIDE_WLAN), true)
        include device/qcom/wlan/default/BoardConfigWlan.mk
    else
        include device/qcom/wlan/hamoa/BoardConfigWlan.mk
    endif
else
    include device/qcom/wlan/hamoa/BoardConfigWlan.mk
endif

-include $(sort $(wildcard vendor/qcom/defs/board-defs/system/*.mk))
-include $(sort $(wildcard vendor/qcom/defs/board-defs/vendor/*.mk))

include device/qcom/sepolicy_vndr/SEPolicy.mk

#Enable dtb in boot image
BOARD_INCLUDE_DTB_IN_BOOTIMG := true
