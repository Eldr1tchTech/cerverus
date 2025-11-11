#pragma once

#include "core/logger.h"

#include <sys/mman.h>
#include <stddef.h>
#include <stdint.h>

// X-Macro for memory tags - easily extensible
#define MEMORY_TAG_LIST \
    X(memory_tag_darray, "DARRAY") \
    X(memory_tag_trie, "TRIE") \
    X(memory_tag_linked_list, "LINKED_LIST") \
    X(memory_tag_string, "STRING") \
    X(memory_tag_free_list, "FREE_LIST") \
    X(memory_tag_unknown, "UNKNOWN")

// Generate enum from X-Macro
typedef enum memory_tag {
#define X(tag, name) tag,
    MEMORY_TAG_LIST
#undef X
    memory_tag_count
} memory_tag;

void cmem_init(size_t size);

void cmem_shutdown();

void* cmem_alloc(memory_tag tag, size_t size);

void cmem_free(memory_tag tag, void* block, size_t size);

// Copy size bytes of memory from src to dest
void cmem_memcpy(void* src_block, void* dest_block, size_t size);

// Set the memory to 0
void cmem_memzero(void* block, size_t size);

// use LOG_DEBUG() to print the amount of bytes allocated for each tag
void cmem_print_stats();