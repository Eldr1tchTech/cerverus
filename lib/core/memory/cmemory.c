#include "cmemory.h"

typedef struct cmem_state
{
    size_t size;
    void* data;
    free_list_node* free_list;
    int sizes[memory_tag_count];
} cmem_state;

static cmem_state* state_ptr = 0;

void cmem_init(size_t size) {
    state_ptr->size = size;
    state_ptr->data = mmap(
        NULL, size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0
    );

    state_ptr->free_list->next = NULL;
    state_ptr->free_list->prev = NULL;
    state_ptr->free_list->offset = 0;
    state_ptr->free_list->size = state_ptr->size;
}

void cmem_shutdown() {
    munmap(state_ptr->data, state_ptr->size);
    // TODO: unallocate free-list.
    state_ptr = 0;
}

void* cmem_alloc(memory_tag tag, size_t size);

void cmem_free(memory_tag, void* block, size_t size);

// Copy size bytes of memory from src to dest
void cmem_memcpy(void* src_block, void* dest_block, size_t size);

// Set the memory to 0
void cmem_memzero(void* block, size_t size);

// use LOG_DEBUG() to print the amount of bytes allocated for each entry in memory tag by the X-macro
void cmem_print_stats();