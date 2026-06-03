/*Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-3-Clause-Clear.  */
/*
 * tmecom_user.h - User-space header for QFPROM fuse operations
 * Version: 1.0
 * Build: Qualcomm Deliverable for Google Factory Integration
 *
 * Notes:
 *   - Must match kernel UAPI layout & ioctl numbers
 *   - addr_type: 0 = RAW, 1 = CORR
 *   - fuse_data[0] = LSB, fuse_data[1] = MSB
 */

#ifndef _TMECOM_USER_H
#define _TMECOM_USER_H

#include <stdint.h>
#include <sys/ioctl.h>

/* Version for traceability */
#define QFPROM_TEST_VERSION "1.0"

/* Magic number - must match kernel implementation */
#define TMECOM_IOCTL_BASE      'T'

/* Address space types for fuse operations */
typedef enum {
    TME_QFPROM_ADDR_SPACE_RAW = 0,   /* Raw address space */
    TME_QFPROM_ADDR_SPACE_CORR,      /* Corrected address space */
    TME_QFPROM_ADDR_SPACE_MAX = 0x7FFFFFFF
} TMEQFPROMAddrSpace_t;

/* Max fuses that can be written in a single operation */
#define TME_MAX_FUSE_WRITE_REQ 64

/* Structure representing a single fuse */
typedef struct {
    uint32_t addr;    /* Fuse address */
    uint32_t data[2]; /* Value to write: data[0] = LSB, data[1] = MSB */
} TMEFuse_t;

/* Fuse Read IOCTL structure */
typedef struct {
    uint32_t addr_type;          /* Address space type (RAW/CORR) */
    uint32_t fuse_addr;          /* Fuse address to read */
    uint32_t fuse_data[2];       /* Fuse value (LSB/MSB) */
    uint32_t qfprom_api_status;  /* QFPROM API status from kernel */
    int ret;                     /* Return status (0 = success) */
} tme_ioctl_fuse_read_t;

/* Fuse Write Multiple IOCTL structure */
typedef struct {
    TMEFuse_t fuse_array[TME_MAX_FUSE_WRITE_REQ]; /* Array of fuses to write */
    uint32_t fuse_array_len;                      /* Number of fuses */
    uint32_t qfprom_api_status;                   /* QFPROM API status */
    int ret;                                      /* Return status (0 = success) */
} tme_ioctl_fuse_write_multiple_t;

/* IOCTL commands */
#define TMECOM_IOCTL_FUSE_READ      _IOWR(TMECOM_IOCTL_BASE, 1, tme_ioctl_fuse_read_t)
#define TMECOM_IOCTL_FUSE_WRITE     _IOWR(TMECOM_IOCTL_BASE, 2, tme_ioctl_fuse_write_multiple_t)

/* Compile-time sanity checks */
_Static_assert(sizeof(((tme_ioctl_fuse_read_t*)0)->fuse_data) == 2 * sizeof(int32_t),
               "fuse_data must be int32_t[2]");
_Static_assert(TME_MAX_FUSE_WRITE_REQ >= 1, "TME_MAX_FUSE_WRITE_REQ must be >= 1");

#endif /* _TMECOM_USER_H */

