#include "cmemory.h"
#include "logger.h"

const int* sizes[] = {0, 0, 0, 0, 0};

void* cmem_allocate(memory_tag tag, size_t size) {
    sizes[tag] += size;
    return malloc(size);
}

// TODO: Eventually track block sizes and implment a static allocator to also track how much memory is currently in use.
void cmem_free(void* block) {
    free(block);
}

void cmem_print_stats() {
    LOG_DEBUG( "\n"
        "   STRING   %iB\n"
        "   ARRAY    %iB\n"
        "   REQUEST  %iB\n"
        "   RESPONSE %iB\n"
        "   NETWORK  %iB\n",
        sizes[0], sizes[1], sizes[2], sizes[3], sizes[4]
    );
}