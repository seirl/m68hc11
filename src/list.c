#include <stdlib.h>
#include <string.h>
#include "list.h"


list* list_init()
{
    list* new = malloc(sizeof(list));
    new->start = NULL;
    new->end = NULL;
    new->count = 0;
    return new;
}

void list_append(list* l, const void* data, const size_t sdata)
{
    list_node* new = malloc(sizeof(list_node));
    new->data = malloc(sdata);
    memcpy(new->data, data, sdata);
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
        p = p->next;
        free(old->data);
        free(old);
    }
    free(l);
}
