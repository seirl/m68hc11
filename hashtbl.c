#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtbl.h"

int hash(int size, char* str)
{
    int h = 1337;
    char c;
    while ((c = (*str++)))
        h = (((h << 5) + h) + c) % size;
    return h;
}

hashtbl* hashtbl_init(int size)
{
    int i;
    hashtbl* tbl = malloc(sizeof(hashtbl));
    tbl->size = size;
    tbl->table = malloc(size * sizeof(list_collisions*));
    for (i = 0; i < size; i++)
        tbl->table[i] = NULL;
    return tbl;
}

void* hashtbl_find(hashtbl* tbl, char* label)
{
    list_collisions* p;
    int h = hash(tbl->size, label);
    p = tbl->table[h];
    while (p)
    {
        if (!strcmp(p->label, label))
            return p->data;
        p = p->next;
    }
    return NULL;
}

list_collisions* _hashtbl_create(char* label, void* data, size_t sdata)
{
    list_collisions* new = malloc(sizeof(list_collisions));
    strcpy(new->label, label);
    new->data = malloc(sdata);
    memcpy(new->data, data, sdata);
    new->next = NULL;
    return new;
}

int hashtbl_add(hashtbl* tbl, char* label, void* data, size_t sdata)
{
    list_collisions* p;
    int h = hash(tbl->size, label);
    list_collisions* new = _hashtbl_create(label, data, sdata);
    p = tbl->table[h];
    if (!p)
        tbl->table[h] = new;
    else
    {
        while (strcmp(p->label, label) && p->next)
            p = p->next;
        if(!strcmp(p->label, label))
            return 1;
        p->next = new;
    }
    return 0;
}

void _hashtbl_remove(list_collisions* elt)
{
    free(elt->data);
    free(elt);
}

int hashtbl_delete(hashtbl* tbl, char* label)
{
    list_collisions* p;
    list_collisions* prev;
    int h = hash(tbl->size, label);
    prev = tbl->table[h];
    if (prev && !strcmp(prev->label, label))
    {
        tbl->table[h] = prev->next;
        _hashtbl_remove(prev);
        return 0;
    }
    while (prev)
    {
        p = prev->next;
        if (p && !strcmp(p->label, label))
        {
            prev->next = p->next;
            _hashtbl_remove(p);
            return 0;
        }
        prev = prev->next;
    }
    return 1;
}

void hashtbl_print(hashtbl* tbl)
{
    int i;
    list_collisions* p;
    for (i = 0; i < tbl->size; i++)
    {
        printf("%d:\n", i);
        p = tbl->table[i];
        while(p)
        {
            printf("\t- %s\n", p->label);
            p = p->next;
        }
    }
}

void hashtbl_destroy(hashtbl* tbl)
{
    int i;
    list_collisions* p;
    list_collisions* old;
    for (i = 0; i < tbl->size; i++)
    {
        p = tbl->table[i];
        tbl->table[i] = NULL;
        while(p)
        {
            old = p;
            p = p->next;
            _hashtbl_remove(old);
        }
    }
    free(tbl->table);
    free(tbl);
}
