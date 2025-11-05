#pragma once

#include <sys/mman.h>

// Forward declaration
typedef struct free_list_node free_list_node;

typedef struct free_list_node
{
    free_list_node* prev;
    size_t offset;
    size_t size;
    free_list_node* next;
} free_list_node;

int fl_node_create(free_list_node* node);

void fl_node_destroy(free_list_node* node);