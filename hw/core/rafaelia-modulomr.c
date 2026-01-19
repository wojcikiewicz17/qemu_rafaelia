/*
 * RAFAELIA ModulomR - Authorship and licensing registry
 */

#include "qemu/osdep.h"
#include "hw/core/rafaelia-modulomr.h"

static const rafaelia_modulomr_entry_t rafaelia_modulomr_table[] = {
    {
        .identifier = "RAFAELIA_BOOTBLOCK_v1",
        .scope = "hw/core/rafaelia-core.*",
        .summary = "Low-level RAFAELIA core formulas and bootblock runtime.",
        .license = "Follow the file header and COPYING/COPYING.LIB.",
        .origin = "Rafaelia authorship (see docs/RAFAELIA_MODULOMR.md).",
    },
    {
        .identifier = "RAFAELIA_MODULOMR",
        .scope = "include/hw/core/rafaelia-modulomr.h, hw/core/rafaelia-modulomr.c",
        .summary = "Authorship and licensing registry module.",
        .license = "Follow the file header and COPYING/COPYING.LIB.",
        .origin = "Rafaelia authorship registry (docs/RAFAELIA_MODULOMR.md).",
    },
};

const rafaelia_modulomr_entry_t *rafaelia_modulomr_entries(size_t *count)
{
    if (count) {
        *count = ARRAY_SIZE(rafaelia_modulomr_table);
    }

    return rafaelia_modulomr_table;
}

const char *rafaelia_modulomr_notice(void)
{
    return "Rafaelia ModulomR registry: see docs/RAFAELIA_MODULOMR.md.";
}
