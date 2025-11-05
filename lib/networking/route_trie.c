#include "route_trie.h"

#include "core/memory/cmemory.h"

void route_trie_create(route_trie* r_trie) {
    r_trie = cmem_alloc(memory_tag_route_trie, sizeof(route_trie));
    r_trie->num_children = 0;
    r_trie->children = NULL;
    r_trie->root_handler = NULL;
}

void route_trie_destroy(route_trie* r_trie) {

}

route_handler route_trie_find_handler(route_trie* trie, http_method method, char* target);

int route_trie_add_route(route_trie* r_trie, http_method method, char* path, route_handler handler);