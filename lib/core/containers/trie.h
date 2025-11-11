#pragma once

#include "darray.h"
#include "core/memory/cmemory.h"

// TODO: Eventually make the key also a void*
// Use the function pointer approach, also predefine some macro setup functions to allow for quicker setup of often used tries

#define DEFAULT_CHILDREN_DARRAY_SIZE 8

typedef int (*key_compare)(void* k1, void* k2);
typedef void* (*key_copy)(void* key);
typedef void (*key_free)(void* key);
typedef size_t (*key_length)(void* key);

typedef struct trie_node
{
    darray* children;
    void* key;
    void* data;
} trie_node;

typedef struct trie
{
    darray* children;
    size_t data_size;

    // Key operations.
    key_compare k_cmp;
    key_copy k_cpy;
    key_free k_free;
    key_length k_len;
} trie;

trie* trie_create(size_t dat_size);

void trie_destroy(trie* t);

trie_node *trie_create_node(trie* t);

// inserts node n at given key, creates any necessary nodes along the way
void trie_insert(trie* t, trie_node* n);

int trie_contains(trie* t, void* k_arg);

// returns the data, or NULL if it could not be found
void* trie_search(trie* t, void* k_arg);