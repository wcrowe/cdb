#ifndef COMMON_H
#define COMMON_H

/* Return codes used by all functions */
#define STATUS_SUCCESS  0
#define STATUS_ERROR    (-1)

/* Magic number that identifies a valid database file */
#define HEADER_MAGIC    0x4c4c4144

/* Fixed field sizes — required by assignment */
#define NAME_LEN        256
#define ADDRESS_LEN     256

/* Database header — exactly 16 bytes on disk (no padding) */
struct dbheader_t {
    unsigned int   magic;     /* Must be HEADER_MAGIC */
    unsigned short version;   /* Currently 1 */
    unsigned short count;     /* Number of employee records */
    unsigned int   filesize;  /* Total file size in bytes */
};

/* Employee record — fixed size */
struct employee_t {
    char name[NAME_LEN];      /* Null-terminated name */
    char address[ADDRESS_LEN];/* Null-terminated address */
    unsigned int hours;       /* Hours worked */
};

#endif /* COMMON_H */