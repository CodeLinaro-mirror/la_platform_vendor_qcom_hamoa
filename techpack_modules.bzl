load("//build/bazel_common_rules/dist:dist.bzl", "copy_to_dist_dir")

def define_techpack_modules(target, msm_target, variant):
    techpack_targets = [
        "//vendor/qcom/opensource/securemsm-kernel:{}_smcinvoke_dlkm".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_qcrypto-msm_dlkm".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_tz_log_dlkm".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_qseecom_dlkm".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_hdcp_qseecom_dlkm".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_qce50_dlkm".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_qcedev-mod_dlkm".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_qrng_dlkm".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_smmu_proxy_dlkm".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_tmecom-intf_dlkm".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_seccam_test_driver".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_hdcp2p2_test".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_si_core_test".format(target),
        "//vendor/qcom/opensource/securemsm-kernel:{}_tornado_mod".format(target),
        "//vendor/qcom/opensource/wlan/platform:{}_cnss2".format(target),
        "//vendor/qcom/opensource/wlan/platform:{}_cnss_plat_ipc_qmi_svc".format(target),
        "//vendor/qcom/opensource/wlan/platform:{}_cnss_nl".format(target),
        "//vendor/qcom/opensource/wlan/platform:{}_cnss_prealloc".format(target),
        "//vendor/qcom/opensource/wlan/platform:{}_cnss_utils".format(target),
        "//vendor/qcom/opensource/wlan/platform:{}_wlan_firmware_service".format(target),
        "//vendor/qcom/opensource/wlan/qcacld-3.0:{}_qca_cld_kiwi-v2".format(target),
    ]

    copy_to_dist_dir(
        name = "{}_all_vendor_module_dist".format(target),
        data = techpack_targets,
        dist_dir = "../device/qcom/hamoa-kernel/techpack",
        flat = True,
        wipe_dist_dir = False,
        allow_duplicate_filenames = False,
        mode_overrides = {"**/*": "644"},
        log = "info",
    )
