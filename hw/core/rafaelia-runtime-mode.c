/*
 * RAFAELIA Runtime mode helpers
 */

#include "qemu/osdep.h"

#include <errno.h>

#include "hw/core/rafaelia-runtime.h"

const char *rafaelia_runtime_mode_name(rafaelia_runtime_mode_t mode)
{
    switch (mode) {
    case RAFAELIA_RUNTIME_MODE_SILENT:
        return "silent";
    case RAFAELIA_RUNTIME_MODE_LOG:
        return "log";
    case RAFAELIA_RUNTIME_MODE_TRACE:
        return "trace";
    case RAFAELIA_RUNTIME_MODE_SYMBIOSIS:
        return "symbiosis";
    case RAFAELIA_RUNTIME_MODE_AUDIT:
        return "audit";
    case RAFAELIA_RUNTIME_MODE_BENCH:
        return "bench";
    default:
        return "unknown";
    }
}

bool rafaelia_runtime_parse_mode(const char *mode_str,
                                 rafaelia_runtime_mode_t *mode)
{
    g_autofree char *mode_stripped = NULL;
    const char *mode_trimmed;
    char *endptr = NULL;
    unsigned long numeric_mode;

    if (!mode_str || !mode) {
        return false;
    }

    mode_stripped = g_strdup(mode_str);
    mode_trimmed = g_strstrip(mode_stripped);
    if (mode_trimmed[0] == '\0') {
        return false;
    }

    if (g_ascii_isdigit(mode_trimmed[0])) {
        errno = 0;
        numeric_mode = strtoul(mode_trimmed, &endptr, 0);
        if (errno != ERANGE && endptr && *endptr == '\0' &&
            numeric_mode <= RAFAELIA_RUNTIME_MODE_BENCH) {
            *mode = (rafaelia_runtime_mode_t)numeric_mode;
            return true;
        }
        return false;
    }

    if (g_ascii_strcasecmp(mode_trimmed, "silent") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_SILENT;
        return true;
    }
    if (g_ascii_strcasecmp(mode_trimmed, "log") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_LOG;
        return true;
    }
    if (g_ascii_strcasecmp(mode_trimmed, "trace") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_TRACE;
        return true;
    }
    if (g_ascii_strcasecmp(mode_trimmed, "symbiosis") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_SYMBIOSIS;
        return true;
    }
    if (g_ascii_strcasecmp(mode_trimmed, "audit") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_AUDIT;
        return true;
    }
    if (g_ascii_strcasecmp(mode_trimmed, "bench") == 0) {
        *mode = RAFAELIA_RUNTIME_MODE_BENCH;
        return true;
    }

    return false;
}
