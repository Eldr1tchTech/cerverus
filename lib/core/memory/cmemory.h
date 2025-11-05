#pragma once

#include "logger.h"

#include <sys/mman.h>

// Use an X-Macro to fill out an enum of memory_tags and create and array of the names

typedef enum memory_tag {
    memory_tag_darray,
    memory_tag_string,
    memory_tag_count
} memory_tag;

typedef struct free_list_node
{
    free_list_node* prev;
    int offset;
    size_t size;
    free_list_node* next;
} free_list_node;

void cmem_init(size_t size);

void cmem_shutdown();

void* cmem_alloc(memory_tag tag, size_t size);

void cmem_free(memory_tag, void* block, size_t size);

// Copy size bytes of memory from src to dest
void cmem_memcpy(void* src_block, void* dest_block, size_t size);

// Set the memory to 0
void cmem_memzero(void* block, size_t size);

// use LOG_DEBUG() to print the amount of bytes allocated for each entry in memory tag by the X-macro
void cmem_print_stats();