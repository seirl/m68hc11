#ifndef HASHTBL
#define HASHTBL

typedef struct list_collisions
{
    char label[100];
    void* data;
    struct list_collisions* next;
} list_collisions;

typedef struct
{
    int size;
    list_collisions** table;
} hashtbl;

hashtbl* hashtbl_init(int size);
void* hashtbl_find(hashtbl* tbl, char* label);
int hashtbl_add(hashtbl* tbl, char* label, void* data, size_t sdata);
int hashtbl_delete(hashtbl* tbl, char* label);
void hashtbl_print(hashtbl* tbl);
void hashtbl_destroy(hashtbl* tbl);

#endif
