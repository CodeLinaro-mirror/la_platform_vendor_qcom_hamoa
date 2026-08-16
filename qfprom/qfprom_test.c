/* Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
SPDX-License-Identifier: BSD-3-Clause-Clear */

/*
 * qfprom_test.c — Qualcomm QFPROM fuse read/write tester
 * Build: gcc -O2 -Wall -Wextra -o qfprom_test qfprom_test.c
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <time.h>

#include "qfprom_test.h"  // Updated header with IOCTL definitions

#ifndef TMECOM_DEVNODE
#define TMECOM_DEVNODE "/dev/tmecom"
#endif

#if defined(TMECOM_IOCTL_FUSE_WRITE)
#  define IOCTL_FUSE_WRITE TMECOM_IOCTL_FUSE_WRITE
#elif defined(TMECOM_IOCTL_FUSE_WRITE_MULTIPLE)
#  define IOCTL_FUSE_WRITE TMECOM_IOCTL_FUSE_WRITE_MULTIPLE
#else
#  error "Header must define TMECOM_IOCTL_FUSE_WRITE or TMECOM_IOCTL_FUSE_WRITE_MULTIPLE"
#endif

#define ANSI_GREEN "\033[1;32m"
#define ANSI_RED   "\033[1;31m"
#define ANSI_RESET "\033[0m"

static int verbose = 0;
static int quiet = 0;
static int dry_run = 0;
static FILE *log_fp = NULL;

__attribute__((unused))static void log_msg(const char *fmt, ...) {
    if (!log_fp) return;
    va_list args;
    va_start(args, fmt);
    vfprintf(log_fp, fmt, args);
    va_end(args);
}

static void usage(const char *p) {
    printf("Usage:\n"
           "  %s [options] fuseread  <addr_type> <fuse_addr>\n"
           "  %s [options] fusewrite <addr> <lsb> <msb> [<addr> <lsb> <msb> ...]\n"
           "\nOptions:\n"
           "  --help       Show usage with examples\n"
           "  --dry-run    Validate arguments without performing IOCTL\n"
           "  --verbose    Show detailed steps\n"
           "  --quiet      Suppress all output except exit code\n"
           "  --log <file> Write debug logs to specified file\n"
           "  --version    Show tool version and build info\n"
           "\nExamples:\n"
           "  Read fuse:  %s fuseread 1 0x1234\n"
           "  Write fuse: %s fusewrite 0x1234 0xCD 0xAB\n",
           p, p, p, p);
}

static void show_version(void) {
    printf("qfprom_test v1.0\n");
}

static int safe_strtoul(const char *s, uint32_t *out) {
    char *endptr = NULL;
    errno = 0;
    unsigned long val = strtoul(s, &endptr, 0);
    if (errno || !endptr || *endptr != '\0' || val > UINT32_MAX) {
        if (!quiet) fprintf(stderr, ANSI_RED "ERROR: Invalid numeric input '%s' (Exit code: 2)\n" ANSI_RESET, s);
        return -1;
    }
    *out = (uint32_t)val;
    return 0;
}

static int open_dev(void) {
    if (verbose && !quiet) printf("Opening device %s...\n", TMECOM_DEVNODE);
    struct stat st;
    if (stat(TMECOM_DEVNODE, &st) != 0) {
        if (!quiet) fprintf(stderr, ANSI_RED "ERROR: Device node %s not found: %s (Exit code: 1)\n" ANSI_RESET,
                            TMECOM_DEVNODE, strerror(errno));
        return -1;
    }
    int fd = open(TMECOM_DEVNODE, O_RDWR | O_CLOEXEC);
    if (fd < 0 && !quiet)
        fprintf(stderr, ANSI_RED "ERROR: open(%s) failed: %s (Exit code: 1)\n" ANSI_RESET,
                TMECOM_DEVNODE, strerror(errno));
    return fd;
}

static void print_fuse64(const char* label, const uint32_t data[2]) {
    if (!quiet) {
        printf("%s: 0x%08x%08x (MSB=0x%08x, LSB=0x%08x)\n",
               label, (uint32_t)data[1], (uint32_t)data[0],
               (uint32_t)data[1], (uint32_t)data[0]);
    }
}

static int do_fuseread(uint32_t addr_type, uint32_t fuse_addr) {
    if (dry_run) {
        if (!quiet) printf("[DRY-RUN] Would read addr_type=%u addr=0x%08x\n", addr_type, fuse_addr);
        return 0;
    }

    int fd = open_dev();
    if (fd < 0) return 1;

    tme_ioctl_fuse_read_t req = {0};
    req.addr_type = addr_type;
    req.fuse_addr = fuse_addr;

    if (verbose && !quiet) printf("Preparing IOCTL request...\n");

    if (ioctl(fd, TMECOM_IOCTL_FUSE_READ, &req) < 0) {
        if (!quiet) fprintf(stderr, ANSI_RED "ERROR: ioctl(TMECOM_IOCTL_FUSE_READ) failed: %s (Exit code: 1)\n" ANSI_RESET,
                            strerror(errno));
        close(fd);
        return 1;
    }

    if (!quiet) {
        printf(ANSI_GREEN "READ SUCCESS\n" ANSI_RESET);
        printf("Address Type: %u\nFuse Addr: 0x%08x\nQFPROM Status: 0x%08x\n", req.addr_type, req.fuse_addr, req.qfprom_api_status);
        print_fuse64("Fuse Value", req.fuse_data);
    }

    close(fd);
    return req.ret ? 1 : 0;
}

static int do_fusewrite_multi(const TMEFuse_t *arr, uint32_t arr_len) {
    if (dry_run) {
        if (!quiet) {
            printf("[DRY-RUN] Would write %u fuses:\n", arr_len);
            for (uint32_t i = 0; i < arr_len; i++)
                printf("  Addr=0x%08x Value=0x%08x%08x (MSB=0x%08x, LSB=0x%08x)\n",
                       arr[i].addr, arr[i].data[1], arr[i].data[0], arr[i].data[1], arr[i].data[0]);
        }
        return 0;
    }

    int fd = open_dev();
    if (fd < 0) return 1;

    tme_ioctl_fuse_write_multiple_t req = {0};
    req.fuse_array_len = arr_len;
    memcpy(req.fuse_array, arr, arr_len * sizeof(TMEFuse_t));

    if (verbose && !quiet) printf("Sending IOCTL for %u fuses...\n", arr_len);

    if (ioctl(fd, IOCTL_FUSE_WRITE, &req) < 0) {
        if (!quiet) fprintf(stderr, ANSI_RED "ERROR: ioctl(IOCTL_FUSE_WRITE) failed: %s (Exit code: 1)\n" ANSI_RESET,
                            strerror(errno));
        close(fd);
        return 1;
    }

    if (!quiet) printf(ANSI_GREEN "WRITE SUCCESS: count=%u\n" ANSI_RESET, req.fuse_array_len);

    close(fd);
    return req.ret ? 1 : 0;
}

int main(int argc, char **argv) {
    if (argc < 2) { usage(argv[0]); return 2; }

    // Parse global options
    int argi = 1;
    for (; argi < argc && argv[argi][0] == '-'; argi++) {
        if (strcmp(argv[argi], "--help") == 0) { usage(argv[0]); return 0; }
        if (strcmp(argv[argi], "--version") == 0) { show_version(); return 0; }
        if (strcmp(argv[argi], "--dry-run") == 0) { dry_run = 1; continue; }
        if (strcmp(argv[argi], "--verbose") == 0) { verbose = 1; continue; }
        if (strcmp(argv[argi], "--quiet") == 0) { quiet = 1; continue; }
        if (strcmp(argv[argi], "--log") == 0 && argi + 1 < argc) {
            log_fp = fopen(argv[++argi], "w");
            if (!log_fp) fprintf(stderr, "Warning: Cannot open log file %s\n", argv[argi]);
            continue;
        }
    }

    if (argi >= argc) { usage(argv[0]); return 2; }

    const char *cmd = argv[argi++];
    if (strcmp(cmd, "fuseread") == 0) {
        if (argc - argi != 2) { usage(argv[0]); return 2; }
        uint32_t addr_type, fuse_addr;
        if (safe_strtoul(argv[argi++], &addr_type) || safe_strtoul(argv[argi++], &fuse_addr)) return 2;
        return do_fuseread(addr_type, fuse_addr);
    }

    if (strcmp(cmd, "fusewrite") == 0) {
        int n = argc - argi;
        if (n < 3 || (n % 3) != 0) {
            if (!quiet) fprintf(stderr, ANSI_RED "ERROR: fusewrite expects triples <addr> <lsb> <msb> (Exit code: 2)\n" ANSI_RESET);
            return 2;
        }
        int trip = n / 3;
        if (trip > (int)TME_MAX_FUSE_WRITE_REQ) {
            if (!quiet) fprintf(stderr, ANSI_RED "ERROR: Too many entries: %d (max %u) (Exit code: 2)\n" ANSI_RESET,
                                trip, TME_MAX_FUSE_WRITE_REQ);
            return 2;
        }

        TMEFuse_t arr[TME_MAX_FUSE_WRITE_REQ] = {0};
        for (int i = 0; i < trip; i++) {
            if (safe_strtoul(argv[argi++], &arr[i].addr) ||
                safe_strtoul(argv[argi++], &arr[i].data[0]) ||
                safe_strtoul(argv[argi++], &arr[i].data[1])) return 2;
        }

        return do_fusewrite_multi(arr, (uint32_t)trip);
    }

    usage(argv[0]);
    return 2;
}

