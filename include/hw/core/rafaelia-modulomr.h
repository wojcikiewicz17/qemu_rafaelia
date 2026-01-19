/*
 * RAFAELIA ModulomR - Authorship and licensing registry
 *
 * This module centralizes references to authored changes, licensing notes,
 * and documentation pointers for RAFAELIA-specific additions.
 */

#ifndef HW_RAFAELIA_MODULOMR_H
#define HW_RAFAELIA_MODULOMR_H

#include <stddef.h>

#define RAFAELIA_MODULOMR_VERSION "1.0"

typedef struct rafaelia_modulomr_entry {
    const char *identifier;
    const char *scope;
    const char *summary;
    const char *license;
    const char *origin;
} rafaelia_modulomr_entry_t;

const rafaelia_modulomr_entry_t *rafaelia_modulomr_entries(size_t *count);
const char *rafaelia_modulomr_notice(void);

#endif /* HW_RAFAELIA_MODULOMR_H */
