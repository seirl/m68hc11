#ifndef LIST
#define LIST

typedef struct list_node
{
    void* data;
    struct list_node* next;
    struct list_node* prev;
} list_node;

typedef struct list
{
    list_node* start;
    list_node* end;
    int count;
} list;

list* list_init();
void list_destroy(list* l);
void list_append(list* l, void* data, size_t sdata);

#endif
