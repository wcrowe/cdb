#ifndef FILE_H
#define FILE_H

int create_db_file(char *filename);
int open_db_file(char *filename);

#define PERM_644 (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)   // rw-r--r--
#define PERM_755 (S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)  // rwxr-xr-x
#define PERM_600 (S_IRUSR|S_IWUSR)                    // rw-------
#endif
