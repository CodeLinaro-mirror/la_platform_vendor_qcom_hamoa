$(call inherit-product, device/qcom/vendor-common/base.mk)

# For PRODUCT_COPY_FILES, the first instance takes precedence.
# Since we want use QC specific files, we should inherit
# device-vendor.mk first to make sure QC specific files gets installed.
$(call inherit-product-if-exists, $(QCPATH)/common/config/device-vendor-qssi.mk)

PRODUCT_COPY_FILES += \
    device/qcom/vendor-common/privapp-permissions-qti-vendor.xml:vendor/etc/permissions/privapp-permissions-qti-vendor.xml
