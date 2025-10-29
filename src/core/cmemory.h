#pragma once

#include <stdlib.h>

typedef enum memory_tag {
    MEMORY_TAG_STRING = 0,
    MEMORY_TAG_ARRAY = 1,
    MEMORY_TAG_REQUEST = 2,
    MEMORY_TAG_RESPONSE = 3,
    MEMORY_TAG_NETWORK = 4,
} memory_tag;

// TODO: Eventually avoid dynamic memory allocations to prevent context switching overhead

void* cmem_allocate(memory_tag tag, size_t size);

void cmem_free(void* block);

void cmem_print_stats();