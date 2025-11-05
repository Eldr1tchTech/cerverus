#include "route_trie.h"

#include "core/memory/cmemory.h"

void route_trie_create(route_trie* r_trie) {
    // TODO: Create the trie with default aka. none data.
}

void route_trie_destroy(route_trie* r_trie) {
    // TODO: Create a recursive trie destroyer,
    // maybe even seperate the trie into a seperate data structure,
    // maybe even do the same for the free_list as a linked list
}

route_handler route_trie_find_handler(route_trie* trie, http_method method, char* path) {
    // TODO: Still need to figure out how exactly static vs dynamic priorities will work out.
    // Will need the path tokenizer for this.
}

int route_trie_add_route(route_trie* r_trie, http_method method, char* path, route_handler handler) {
    // TODO: Just always replace whatever route was there worst case scenario.
}

// TODO: Create a path tokenizer, probably parse it into an darray. Maybe even stick it in the server/router
// Test certain formats, such as how do you receive /api/ versus /api.