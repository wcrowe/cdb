#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

// DO NOT define HEADER_MAGIC here — parse.h owns it (0x4c4c4144)
// DO NOT define NAME_LEN or ADDRESS_LEN here — parse.h owns them too

#define STATUS_SUCCESS  0
#define STATUS_ERROR   -1

struct dbheader_t {
    uint32_t magic;     // 0x4c4c4144 when read from disk (after ntohl)
    uint16_t version;   // 1
    uint16_t count;     // number of employees
    uint32_t filesize;  // total file size in bytes
};

struct employee_t {
    char name[256];        // Must be exactly 256 (test suite checks this)
    char address[256];     // Must be exactly 256
    uint32_t hours;
};

#endif /* COMMON_H */