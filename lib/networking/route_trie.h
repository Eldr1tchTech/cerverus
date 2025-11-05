#pragma once

#include "request.h"
#include "response.h"

typedef void* (*route_handler)(request* req, response* res);

typedef enum route_segment_type {
    route_segment_type_static,
    route_segment_type_dynamic
} route_segment_type;

typedef struct route_trie_node
{
    route_trie_node* children;
    int num_children;
    char* segment;
    route_segment_type type;
    route_handler route_handler;
} route_trie_node;

typedef struct route_trie
{
    route_trie_node* children;
    int num_children;
    route_handler root_handler;
} route_trie;

void route_trie_create(route_trie* r_trie);

void route_trie_destroy(route_trie* r_trie);

route_handler route_trie_find_handler(http_method method, char* target);

int route_trie_add_route(http_method method, char* path, route_handler handler);

void route_trie_set_root_handler(route_handler handler);