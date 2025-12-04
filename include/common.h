#ifndef COMMON_H
#define COMMON_H

#define STATUS_SUCCESS  0
#define STATUS_ERROR   -1

struct dbheader_t {
    unsigned int magic;
    unsigned short version;
    unsigned short count;
    unsigned int filesize;
};

struct employee_t {
    char name[256];
    char address[256];
    unsigned int hours;
};

#endif /* COMMON_H */