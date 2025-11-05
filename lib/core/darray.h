#pragma once

// Create a darray, this is to store the route nodes for the trie mainly,

typedef struct darray
{
    int darr_size;
    int stride;
    int length;
    void* data;
} darray;

void darray_create(darray* darr, int stride, int darr_size);

void darray_resize(darray* darr);

void darray_destroy(darray* darr);