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
    {
        .identifier = "RAFAELIA_INTEGRATION",
        .scope = "include/hw/core/rafaelia-integration.h",
        .summary = "Multi-repository integration hub definitions.",
        .license = "Follow the file header and COPYING/COPYING.LIB.",
        .origin = "Rafaelia integration layer (docs/RAFAELIA_AUTHORSHIP_BOUNDARIES.md).",
    },
    {
        .identifier = "RAFAELIA_MVP_MODULES",
        .scope = "include/hw/core/rafaelia-mvp-modules.h",
        .summary = "MVP module system and aggregates for RAFAELIA.",
        .license = "Follow the file header and COPYING/COPYING.LIB.",
        .origin = "Rafaelia modular layer (docs/RAFAELIA_AUTHORSHIP_BOUNDARIES.md).",
    },
    {
        .identifier = "RAFAELIA_RMR",
        .scope = "include/hw/core/rafaelia-rmr.h, hw/core/rafaelia-rmr.c, hw/core/rafaelia-core.c, docs/RAFAELIA_RMR.md",
        .summary = "Low-overhead runtime memory pool and hardware profile hints.",
        .license = "Follow the file header and COPYING/COPYING.LIB.",
        .origin = "Rafaelia runtime optimization module (docs/RAFAELIA_MODULOMR.md).",
    },
    {
        .identifier = "RAFAELIA_GOVERNANCE_DOCS",
        .scope = "docs/RAFAELIA_AUTHORSHIP_BOUNDARIES.md, docs/RAFAELIA_MODULOMR.md, README_RAFAELIA.md",
        .summary = "Framework layering, authorship boundaries, and governance references.",
        .license = "Follow the file header and COPYING/COPYING.LIB.",
        .origin = "Rafaelia documentation governance (docs/RAFAELIA_AUTHORSHIP_BOUNDARIES.md).",
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
