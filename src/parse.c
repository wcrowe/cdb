// 1. create_db_header — remove fd parameter
int create_db_header(struct dbheader_t **headerOut)
{
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (!header) return STATUS_ERROR;

    header->magic    = HEADER_MAGIC;
    header->version  = 1;
    header->count    = 0;
    header->filesize = sizeof(struct dbheader_t);

    *headerOut = header;
    return STATUS_SUCCESS;
}

// 2. add_employee — employees is now struct employee_t **
int add_employee(struct dbheader_t *dbhdr, struct employee_t **employees, char *addstring)
{
    char buffer[1024];
    strncpy(buffer, addstring, sizeof(buffer)-1);
    buffer[sizeof(buffer)-1] = '\0';

    char *name  = strtok(buffer, ",");
    char *addr  = strtok(NULL, ",");
    char *hours = strtok(NULL, ",");

    if (!name || !addr || !hours) return STATUS_ERROR;

    // Realloc the employee array
    struct employee_t *new_mem = realloc(*employees, (dbhdr->count + 1) * sizeof(struct employee_t));
    if (!new_mem) return STATUS_ERROR;

    *employees = new_mem;
    struct employee_t *emp = &(*employees)[dbhdr->count];

    strncpy(emp->name, name, NAME_LEN-1);
    emp->name[NAME_LEN-1] = '\0';
    strncpy(emp->address, addr, ADDRESS_LEN-1);
    emp->address[ADDRESS_LEN-1] = '\0';
    emp->hours = atoi(hours);

    dbhdr->count++;
    return STATUS_SUCCESS;
}

// 3. list_employees — implement it properly
void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees)
{
    if (!dbhdr || !employees) return;

    printf("\n=== Employee Database (%d record%s) ===\n\n", 
           dbhdr->count, dbhdr->count == 1 ? "" : "s");

    for (int i = 0; i < dbhdr->count; i++) {
        printf("Employee #%d\n", i + 1);
        printf("  Name   : %s\n", employees[i].name);
        printf("  Address: %s\n", employees[i].address);
        printf("  Hours  : %u\n\n", employees[i].hours);
    }
}