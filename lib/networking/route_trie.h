#pragma once

#include "request.h"
#include "response.h"
#include "core/containers/darray.h"

#define DARRAY_DEFAULT_SIZE 8

typedef void* (*route_handler)(request* req, response* res);

typedef enum route_segment_type {
    route_segment_type_static,
    route_segment_type_dynamic
} route_segment_type;

typedef struct route_trie_node
{
    darray* children;
    char* segment;
    route_segment_type type;
    route_handler* route_handler;
} route_trie_node;

// Eventually switch to a hashmap for the 
typedef struct route_trie
{
    darray* children[http_method_unknown];
    route_handler* root_handler;
} route_trie;

void route_trie_create(route_trie* r_trie);

void route_trie_destroy(route_trie* r_trie);

route_handler* route_trie_find_handler(route_trie* r_trie, http_method method, char* path);

void route_trie_add_route(route_trie* r_trie, http_method method, char* path, route_handler* handler);