#pragma once

#include "core/memory/cmemory.h"

typedef struct linked_list_node linked_list_node;

typedef struct linked_list_node
{
    linked_list_node* prev;
    void* data;
    linked_list_node* next;
} linked_list_node;

typedef struct linked_list
{
    linked_list_node* root;
    size_t data_size;
} linked_list;

linked_list* linked_list_create(size_t data_size);

void linked_list_destroy(linked_list* ll);

linked_list_node* linked_list_node_create(linked_list* ll);

void Linked_list_node_destroy(linked_list* ll, linked_list_node* node);

// If prev_node is NULL it is inserted at the very beginning.
void linked_list_insert(linked_list* ll, linked_list_node* prev_node, linked_list_node* node);