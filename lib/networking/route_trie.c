#include "route_trie.h"

#include "core/cmemory.h"

void route_trie_create(route_trie* r_trie) {
    r_trie = cmem_alloc(MEMORY_TAG_ROUTE_TRIE, sizeof(route_trie));
}

void route_trie_destroy(route_trie* r_trie);

int route_trie_add_route(http_method method, char* path, route_handler handler);

route_handler route_trie_find_handler(http_method method, char* target);

void route_trie_set_root_handler(route_trie* r_trie, route_handler handler) {
    r_trie->root_handler = handler;
}