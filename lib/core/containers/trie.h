#pragma once

#include "darray.h"
#include "core/memory/cmemory.h"

// TODO: Eventually make the key also a void*

#define DEFAULT_CHILDREN_DARRAY_SIZE 8

typedef struct trie_node
{
    darray* children;
    char* key;
    void* data;
} trie_node;

typedef struct trie
{
    darray* children;
    size_t data_size;
} trie;

trie* trie_create(size_t dat_size);

void trie_destroy(trie* t);

trie_node *trie_create_node(trie* t);

// inserts said node at given key, creates any necessary nodes along the way
void trie_insert(trie* t, trie_node* n);

int trie_contains(trie* t, char* k_arg);

// returns the data, or NULL if it could not be found
void* trie_search(trie* t, char* k_arg);

// Maybe coalesce unused nodes?
// But then again coding a delete is already somewhat unnecessary as far as the tries planned use-case goes
void trie_delete(trie* t, char* k_arg);