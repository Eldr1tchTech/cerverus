#pragma once

#include "core/containers/trie.h"
#include "networking_types.inl"

typedef void (*route_handler)(request* req, response* res);

typedef struct routing_table
{
    trie* route_trie;
} routing_table;

void routing_table_add_route(http_method method, char* target, route_handler* handler);