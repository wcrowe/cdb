#ifndef COMMON_H
#define COMMON_H

#define STATUS_ERROR   -1
#define STATUS_SUCCESS 0

#define PERM_644 (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)   // rw-r--r--
#define PERM_755 (S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)  // rwxr-xr-x
#define PERM_600 (S_IRUSR|S_IWUSR)                    // rw-------

#endif
