#pragma once

#include "core/containers/trie.h"

typedef struct routing_table
{
    trie* route_trie;
} routing_table;

