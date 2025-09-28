load("//build/bazel_common_rules/dist:dist.bzl", "copy_to_dist_dir")

def define_techpack_modules(target, msm_target, variant):
    techpack_targets = [

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
