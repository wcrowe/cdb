#ifndef COMMON_H
#define COMMON_H

#define STATUS_SUCCESS  0
#define STATUS_ERROR   -1

struct dbheader_t {
    unsigned int magic;     // 4 bytes
    unsigned short version; // 2 bytes
    unsigned short count;   // 2 bytes
    unsigned int filesize;  // 4 bytes → total 16 bytes, no padding
};

struct employee_t {
    char name[256];
    char address[256];
    unsigned int hours;
};

#endif /* COMMON_H */