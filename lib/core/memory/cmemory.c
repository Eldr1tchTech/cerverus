#include "cmemory.h"

#include "free_list.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>

// Generate string array from X-Macro
static const char* memory_tag_names[] = {
#define X(tag, name) name,
    MEMORY_TAG_LIST
#undef X
};

typedef struct cmem_state
{
    size_t total_size;
    void* data;
    free_list_node* free_list;
    size_t sizes[memory_tag_count];
    size_t allocations[memory_tag_count];
} cmem_state;

static cmem_state* state_ptr = NULL;

// Helper function to coalesce adjacent free blocks
static void coalesce_free_blocks() {
    if (!state_ptr || !state_ptr->free_list) return;
    
    free_list_node* current = state_ptr->free_list;
    
    while (current && current->next) {
        // Check if current block is adjacent to next block
        if (current->offset + current->size == current->next->offset) {
            // Merge blocks
            free_list_node* to_remove = current->next;
            current->size += to_remove->size;
            current->next = to_remove->next;
            
            if (current->next) {
                current->next->prev = current;
            }
            
            // Free the node structure (we'll need a separate allocation for these)
            // For simplicity, we're using the system allocator for the free list nodes
            fl_node_destroy(to_remove);
        } else {
            current = current->next;
        }
    }
}

void cmem_init(size_t size) {
    // Allocate the state structure itself using mmap
    state_ptr = (cmem_state*)mmap(
        NULL, sizeof(cmem_state),
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0
    );
    
    if (state_ptr == MAP_FAILED) {
        LOG_ERROR("cmem_init - Failed to allocate memory for state structure");
        state_ptr = NULL;
        return;
    }
    
    // Initialize state
    state_ptr->total_size = size;
    
    // Allocate the main memory pool
    state_ptr->data = mmap(
        NULL, size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0
    );
    
    if (state_ptr->data == MAP_FAILED) {
        LOG_ERROR("Failed to allocate memory pool of size %zu", size);
        munmap(state_ptr, sizeof(cmem_state));
        state_ptr = NULL;
        return;
    }
    
    // Initialize statistics
    for (int i = 0; i < memory_tag_count; i++) {
        state_ptr->sizes[i] = 0;
        state_ptr->allocations[i] = 0;
    }
    
    // Create initial free list node
    if (fl_node_create(state_ptr->free_list)) {
        LOG_ERROR("Failed to allocate initial free list node");
        munmap(state_ptr->data, size);
        munmap(state_ptr, sizeof(cmem_state));
        state_ptr = NULL;
        return;
    }
    
    state_ptr->free_list->prev = NULL;
    state_ptr->free_list->next = NULL;
    state_ptr->free_list->offset = 0;
    state_ptr->free_list->size = state_ptr->total_size;
    
    LOG_DEBUG("Memory system initialized with %zu bytes", size);
}

void cmem_shutdown() {
    if (!state_ptr) return;
    
    // Print final stats before shutdown
    cmem_print_stats();
    
    // Free all free list nodes
    free_list_node* current = state_ptr->free_list;
    while (current) {
        free_list_node* next = current->next;
        fl_node_destroy(current);
        current = next;
    }
    
    // Free the main memory pool
    munmap(state_ptr->data, state_ptr->total_size);
    
    // Free the state structure
    munmap(state_ptr, sizeof(cmem_state));
    
    state_ptr = NULL;
    
    LOG_DEBUG("Memory system shut down");
}

void* cmem_alloc(memory_tag tag, size_t size) {
    if (!state_ptr || !state_ptr->data) {
        LOG_ERROR("Memory system not initialized");
        return NULL;
    }
    
    // Align size to 8 bytes for better alignment
    size = (size + 7) & ~7;
    
    // Find a suitable free block using first-fit algorithm
    free_list_node* current = state_ptr->free_list;
    free_list_node* best_fit = NULL;
    
    while (current) {
        if (current->size >= size) {
            // Found a suitable block
            best_fit = current;
            break;  // Use first-fit for simplicity
        }
        current = current->next;
    }
    
    if (!best_fit) {
        LOG_ERROR("Out of memory! Requested %zu bytes for tag %s", 
                  size, memory_tag_names[tag]);
        return NULL;
    }
    
    // Calculate the pointer to return
    void* ptr = (uint8_t*)state_ptr->data + best_fit->offset;
    
    // Update statistics
    state_ptr->sizes[tag] += size;
    state_ptr->allocations[tag]++;
    
    // Update the free list
    if (best_fit->size == size) {
        // Exact fit - remove this node from free list
        if (best_fit->prev) {
            best_fit->prev->next = best_fit->next;
        } else {
            state_ptr->free_list = best_fit->next;
        }
        
        if (best_fit->next) {
            best_fit->next->prev = best_fit->prev;
        }
        
        fl_node_destroy(best_fit);
    } else {
        // Split the block
        best_fit->offset += size;
        best_fit->size -= size;
    }
    
    return ptr;
}

void cmem_free(memory_tag tag, void* block, size_t size) {
    if (!state_ptr || !block) return;
    
    // Align size to 8 bytes (same as in alloc)
    size = (size + 7) & ~7;
    
    // Calculate offset
    size_t offset = (uint8_t*)block - (uint8_t*)state_ptr->data;
    
    // Validate the block is within our memory pool
    if (offset >= state_ptr->total_size) {
        LOG_ERROR("Attempted to free invalid memory block");
        return;
    }
    
    // Update statistics
    state_ptr->sizes[tag] -= size;
    state_ptr->allocations[tag]--;

    // Zero Memory
    
    
    // Create a new free list node
    free_list_node* new_node;
    fl_node_create(new_node);
    
    if (new_node == MAP_FAILED) {
        LOG_ERROR("Failed to allocate free list node during free");
        return;
    }
    
    new_node->offset = offset;
    new_node->size = size;
    
    // Insert into free list in sorted order (by offset)
    if (!state_ptr->free_list || offset < state_ptr->free_list->offset) {
        // Insert at beginning
        new_node->prev = NULL;
        new_node->next = state_ptr->free_list;
        if (state_ptr->free_list) {
            state_ptr->free_list->prev = new_node;
        }
        state_ptr->free_list = new_node;
    } else {
        // Find insertion point
        free_list_node* current = state_ptr->free_list;
        while (current->next && current->next->offset < offset) {
            current = current->next;
        }
        
        // Insert after current
        new_node->prev = current;
        new_node->next = current->next;
        if (current->next) {
            current->next->prev = new_node;
        }
        current->next = new_node;
    }
    
    // Coalesce adjacent free blocks
    // TODO: Maybe increase performance by only checking adjacent nodes, maybe even pass a parameter for the prev
    coalesce_free_blocks();
}

void cmem_memcpy(void* src_block, void* dest_block, size_t size) {
    if (!src_block || !dest_block || size == 0) return;
    
    uint8_t* src = (uint8_t*)src_block;
    uint8_t* dest = (uint8_t*)dest_block;
    
    // Simple byte-by-byte copy
    // Could be optimized with larger chunks
    for (size_t i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

void cmem_memzero(void* block, size_t size) {
    if (!block || size == 0) return;
    
    uint8_t* ptr = (uint8_t*)block;
    
    // Set memory to zero
    for (size_t i = 0; i < size; i++) {
        ptr[i] = 0;
    }
}

void cmem_print_stats() {
    if (!state_ptr) {
        LOG_DEBUG("Memory system not initialized");
        return;
    }
    
    LOG_DEBUG("=== Memory Statistics ===");
    LOG_DEBUG("Total memory pool size: %zu bytes", state_ptr->total_size);
    
    size_t total_allocated = 0;
    for (int i = 0; i < memory_tag_count; i++) {
        if (state_ptr->sizes[i] > 0) {
            LOG_DEBUG("  %s: %zu bytes (%zu allocations)", 
                     memory_tag_names[i], 
                     state_ptr->sizes[i], 
                     state_ptr->allocations[i]);
            total_allocated += state_ptr->sizes[i];
        }
    }
    
    LOG_DEBUG("Total allocated: %zu bytes", total_allocated);
    LOG_DEBUG("Total free: %zu bytes", state_ptr->total_size - total_allocated);
    
    // Count free blocks
    int free_block_count = 0;
    free_list_node* current = state_ptr->free_list;
    while (current) {
        free_block_count++;
        current = current->next;
    }
    LOG_DEBUG("Free blocks: %d", free_block_count);
    LOG_DEBUG("========================");
}