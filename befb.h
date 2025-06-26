/**************************************************************
 *    Archana Berry OS - Project Source Code                  *
 *                                                            *
 *    Filename   : befb.h                                     *
 *    Component  : BEFB Loadee Core                           *
 *    Description: Maker file BEFB binary and segment map     *
 *                                                            *
 *    Copyright © 2025 Archana Berry Project OS Devs          *
 *    Licensed under ABRL v1.0 - Research License             *
 *                                                            *
 *    You may not use this file except in compliance with     *
 *    the License.                          				  *
 *                                                            *
 *    Unless required by applicable law or agreed to in       *
 *    writing, software distributed under the License is      *
 *    distributed on an "AS IS" BASIS, WITHOUT WARRANTIES     *
 *    OR CONDITIONS OF ANY KIND, either express or implied.   *
 **************************************************************/

#ifndef BEFB_H
#define BEFB_H

#include <stdint.h>

/*
 * BEFB Magic Code Format: "BEFBxyzA"
 *
 *   BEFB           ← Prefix (Berry Executable File Binary)
 *   x              ← Arch (1=CISC, 2=RISC, 3=RISC-V)
 *   y+z = yz       ← Bit class (e.g. y=1,z=6 → 16-bit; y=3,z=2 → 32-bit; y=6,z=4 → 64-bit)
 *   A              ← ABI target (1=BLDOS, 2=BEFB Native, 3=ABRT Runtime, 4=ABVM Virtual, 5=RTAP)
 *
 * Example:
 *   "BEFB1321" → CISC, 32-bit, BLDOS
 *   "BEFB2644" → RISC, 64-bit, ABVM
 */

#define BEFB_MAGIC_LEN       8
#define BEFB_MAX_NAME_LEN   64

#pragma pack(push,1)

/* Main BEFB header */
typedef struct befb_header {
    char     magic[BEFB_MAGIC_LEN];   /* "BEFBxyzA" */
    uint8_t  arch;                    /* x: 1=CISC,2=RISC,3=RISC-V */
    uint8_t  bit_start;               /* y */
    uint8_t  bit_end;                 /* z */
    uint8_t  abi;                     /* A: target ABI */

    uint32_t header_size;             /* sizeof(befb_header) */
    uint32_t version_major;           /* Major version */
    uint32_t version_minor;           /* Minor version */
    uint32_t flags;                   /* Feature flags */

    /* Segment table */
    uint32_t seg_table_offset;        /* Offset from file start */
    uint32_t seg_count;               /* Number of segment entries */

    /* CherryReva (fixup) table */
    uint32_t rev_table_offset;
    uint32_t rev_count;

    /* Plugin table */
    uint32_t plug_table_offset;
    uint32_t plug_count;

    /* Debug map table */
    uint32_t dbg_table_offset;
    uint32_t dbg_count;

    /* Icon table */
    uint32_t icon_table_offset;
    uint32_t icon_count;

    /* Manifest (JSON-like) */
    uint32_t manifest_offset;
    uint32_t manifest_size;

    /* Hash & signature */
    uint32_t hash_offset;
    uint32_t hash_size;
    uint32_t sig_offset;
    uint32_t sig_size;

    uint32_t eof_marker;              /* Should be 0x454F4621 ("EOF!") */
} befb_header_t;

/* Per-segment entry */
typedef struct befb_segment_entry {
    char     name[BEFB_MAX_NAME_LEN]; /* E.g. ".text", ".data", etc. */
    uint32_t file_offset;             /* Offset in .berrysegs/<segment> */
    uint32_t file_size;               /* Raw size on disk */
    uint32_t mem_size;                /* Size when loaded (for .bss may > file_size) */
    uint64_t virt_addr;               /* Load address in memory */
    uint32_t flags;                   /* Segment flags (R/W/X) */
} befb_segment_entry_t;

/* CherryReva: RVA & base patch */
typedef struct cherryreva_entry {
    uint32_t table_index;             /* Index into segment table */
    uint64_t orig_rva;                /* Original relative virtual address */
    uint64_t new_base;                /* Base to apply */
} cherryreva_entry_t;

/* Plugin entry */
typedef struct befb_plugin_entry {
    char     name[BEFB_MAX_NAME_LEN]; /* Plugin filename */
    uint32_t offset;                  /* Offset into plugin blob */
    uint32_t size;                    /* Size of plugin data */
} befb_plugin_entry_t;

/* Debug map entry */
typedef struct befb_dbg_entry {
    char     symbol[BEFB_MAX_NAME_LEN];
    uint64_t address;                 /* Virtual address of symbol */
    uint32_t src_line;                /* Source line for debugging */
} befb_dbg_entry_t;

/* Icon entry */
typedef struct befb_icon_entry {
    uint32_t icon_id;                 /* Numeric icon ID */
    uint32_t offset;                  /* Offset into icon data blob */
    uint32_t size;                    /* Icon file size */
    uint16_t width;                   /* Pixel width */
    uint16_t height;                  /* Pixel height */
} befb_icon_entry_t;

/* Hash entry */
typedef struct befb_hash {
    uint8_t  algorithm;               /* Hash algorithm ID */
    uint8_t  digest[64];              /* Max 512-bit digest */
} befb_hash_t;

/* Signature entry */
typedef struct befb_signature {
    uint8_t  algorithm;               /* Signature algorithm ID */
    uint8_t  signature[512];          /* Signature blob */
    uint32_t sig_len;                 /* Actual signature length */
} befb_signature_t;

#pragma pack(pop)

/* Utility to validate magic code */
static inline int befb_validate_magic(const befb_header_t *hdr) {
    /* Expect first 4 bytes to be 'B','E','F','B' */
    return (hdr->magic[0]=='B' && hdr->magic[1]=='E' &&
            hdr->magic[2]=='F' && hdr->magic[3]=='B');
}

/* Function prototypes */
int  befb_load(const char *path);
void befb_free(void);

#endif /* BEFB_H */
