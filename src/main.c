#include "core/logger.h"

int main () {
    LOG_ERROR("Error message woohooo!");
    LOG_DEBUG("Debug Test: %s", "passed");

    return 0;
}