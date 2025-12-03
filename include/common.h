#ifndef COMMON_H
#define COMMON_H

#define HEADER_MAGIC   0xdeadbeef
#define STATUS_SUCCESS 0
#define STATUS_ERROR   -1
#define PERM_644       0644

#include <stdint.h>

struct dbheader_t {
    uint32_t magic;
    uint16_t version;
    uint16_t count;
    uint32_t filesize;
};

struct employee_t {
    char name[256];        // or whatever size you want — but only defined ONCE
    char address[256];
    uint32_t hours;
};

#endif