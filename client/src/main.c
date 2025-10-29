#include <core/logger.h>
#include <core/cmemory.h>

#include <string.h>

int main () {
    cmem_print_stats();
    char* block = cmem_allocate(MEMORY_TAG_STRING, sizeof(char) * 8);
    
    strcpy(block, "Hello!");

    cmem_print_stats();

    return 0;
}