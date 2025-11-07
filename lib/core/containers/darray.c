#include "darray.h"

#include "cmemory.h"
#include "logger.h"

void darray_create(darray* darr, int stride, int darr_size) {
    darr = cmem_alloc(memory_tag_darray, sizeof(darr));

    darr->darr_size = darr_size;
    darr->stride = stride;
    darr->length = 0;
    darr->data = cmem_alloc(memory_tag_darray, darr_size * stride);
}

void darray_resize(darray* darr, int new_size) {
    if (new_size == 0)
    {
        new_size = darr->darr_size * 2;
    }
    
    void* new_data = cmem_alloc(memory_tag_darray, new_size * darr->stride);

    cmem_memcpy(darr->data, new_data, darr->darr_size * darr->stride);
    cmem_free(memory_tag_darray, darr->data, darr->darr_size * darr->stride);

    darr->data = new_data;
    darr->darr_size = new_size;
}

void darray_destroy(darray* darr) {
    cmem_free(memory_tag_darray, darr->data, darr->darr_size * darr->stride);
    cmem_free(memory_tag_darray, darr, sizeof(darray));
    darr = 0;
}

void darray_add(darray* darr, void* item) {
    if (darr->length == darr->darr_size)
    {
        darray_resize(darr, 0);
    }

    cmem_memcpy(item, darr->data + darr->stride * darr->length, darr->stride);
    darr->length++;
}