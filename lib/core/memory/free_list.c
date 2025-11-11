#include "free_list.h"

#include <stddef.h>

int fl_node_create(free_list_node* node) {
    node = (free_list_node*)mmap(
        NULL, sizeof(free_list_node),
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0
    );

    if (node == MAP_FAILED) {
        LOG_ERROR("fl_node_create - Failed to allocate free list node");
        return 0;
    }

    return 1;
}

void fl_node_destroy(free_list_node* node) {
    munmap(node, sizeof(free_list_node));
    node = 0;
}
