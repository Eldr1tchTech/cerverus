#pragma once

// Create a darray, this is to store the route nodes for the trie mainly,

typedef struct darray
{
    int darr_size;
    size_t stride;
    int length;
    void* data;
} darray;

void darray_create(darray* darr, size_t stride, int darr_size);

// If 0 is passed as new_size, the darray will double in size.
void darray_resize(darray* darr, int new_size);

void darray_destroy(darray* darr);

// copies the memory from item to the end of the darray,
// if it is too small it resizes it with 0 passed as new_size.
void darray_add(darray* darr, void* item);