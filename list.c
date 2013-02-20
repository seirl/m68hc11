#include <stdlib.h>
#include "list.h"


list* list_init()
{
    list* new = malloc(sizeof(list));
    new->start = NULL;
    new->end = NULL;
    new->count = 0;
    return new;
}

void list_append(list* l, void* data, size_t sdata)
{
    list_node* new = malloc(sdata);
    new->data = data;
    new->next = NULL;
    new->prev = l->end;
    if(!l->start || !l->end)
        l->start = new;
    else
        l->end->next = new;
    l->end = new;
    l->count++;
}

void list_destroy(list* l)
{
    list_node* p;
    list_node* old;
    p = l->start;
    while (p)
    {
        old = p;
        free(old->data);
        free(old);
        p = p->next;
    }
    free(l);
}
