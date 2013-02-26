#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtbl.h"

/* Hash function (using the djb2 algorithm) with excellent distribution */
int hash(const int size, const char* str)
{
    int h = 1337;
    char c;
    while ((c = (*str++)))
        h = (((h << 5) + h) + c) % size;
    return h;
}

/* Initialize the data type */
hashtbl* hashtbl_init(const int size)
{
    int i;
    hashtbl* tbl = malloc(sizeof(hashtbl));
    tbl->size = size;
    tbl->table = calloc(size, sizeof(list_collisions*));
    for (i = 0; i < size; i++)
        tbl->table[i] = NULL;
    return tbl;
}

/* Find an element in the hashtable by its label */
void* hashtbl_find(hashtbl* tbl, const char* label)
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

/* Intermediate function used to create an element in the table */
list_collisions* _hashtbl_create(const char* label, const void* data,
        const size_t sdata)
{
    list_collisions* new = malloc(sizeof(list_collisions));
    strcpy(new->label, label);
    new->data = malloc(sdata);
    memcpy(new->data, data, sdata);
    new->next = NULL;
    return new;
}

/* Add an element in the table */
int hashtbl_add(hashtbl* tbl, const char* label, const void* data,
        const size_t sdata)
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

/* Intermediate function used to remove an element in the table */
void _hashtbl_remove(list_collisions* elt)
{
    free(elt->data);
    free(elt);
}

/* Remove an element in the table */
int hashtbl_delete(hashtbl* tbl, const char* label)
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

/* Display an ASCII representation of the hashtable, useful for debug */
void hashtbl_print(const hashtbl* tbl)
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

/* Destroy the hashtable */
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
