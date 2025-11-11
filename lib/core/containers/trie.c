#include "trie.h"

#include <string.h>

trie* trie_create(size_t dat_size)
{
    trie *t = (trie *)cmem_alloc(memory_tag_trie, sizeof(trie));
    t->data_size = dat_size;
    darray_create(t->children, sizeof(trie_node *), DEFAULT_CHILDREN_DARRAY_SIZE);
}

void trie_node_destroy (trie_node* node, size_t dat_size) {
    if (!node) return;

    darray *curr_darr = node->children;
    trie_node **n_data = curr_darr->data;
    for (int i = 0; i < curr_darr->length; i++)
    {
        trie_node_destroy(n_data[i], dat_size);
    }
    
    cmem_free(memory_tag_trie, node->data, dat_size);
    node->data;
    node->key = 0;
    darray_destroy(curr_darr);
    node->children = 0;
    cmem_free(memory_tag_trie, node, sizeof(trie_node));
    node = 0;
}

// make some sort of recursive function
void trie_destroy(trie *t) {
    darray *curr_darr = t->children;
    trie_node **n_data = curr_darr->data;
    for (int i = 0; i < curr_darr->length; i++)
    {
        trie_node_destroy(n_data[i], t->data_size);
    }
    
    darray_destroy(curr_darr);
    t->data_size = 0;
    cmem_free(memory_tag_trie, t, sizeof(trie));
    t = 0;
}

trie_node *trie_create_node(trie *t)
{
    trie_node *new_node = (trie_node *)cmem_alloc(memory_tag_trie, sizeof(trie_node));
    darray_create(new_node->children, sizeof(trie_node *), DEFAULT_CHILDREN_DARRAY_SIZE);
    new_node->data = cmem_alloc(memory_tag_trie, t->data_size);

    return new_node;
}

// inserts said node at given key, creates any necessary nodes along the way
void trie_insert(trie *t, trie_node *n)
{
    darray *curr_darr = t->children;
    trie_node **n_data = curr_darr->data;
    size_t key_len = strlen(n->key);
    for (int i = 0; i < key_len; i++)
    {
        for (int j = 0; j < curr_darr->length; j++)
        {
            if (n->key == n_data[j]->key)
            {
                if (i == key_len - 1)
                {
                    darray_add(n_data[j]->children, n);
                    return;
                }
                else
                {
                    curr_darr = n_data[j]->children;
                    n_data = curr_darr->data;
                }
            }
            else
            {
                trie_node* new_node = trie_create_node(t);
                new_node->key = n->key;
                darray_add(n_data[j]->children, new_node);
                curr_darr = new_node->children;
                n_data = curr_darr->data;
            }
        }
    }
}

// returns the data, or NULL if it could not be found
void *trie_search(trie *t, void *k_arg)
{
    darray *curr_darr = t->children;
    trie_node **n_data = curr_darr->data;
    int on_track = 1;
    size_t key_len = t->k_len(k_arg);
    for (int i = 0; i < key_len && on_track; i++)
    {
        on_track = 0;
        for (int j = 0; j < curr_darr->length && !on_track; j++)
        {
            if (t->k_cmp(k_arg[i], n_data[j]->key) == 0)
            {
                if (i == key_len - 1)
                {
                    return n_data[j]->data;
                }
                else
                {
                    on_track = 1;
                    curr_darr = n_data[j]->children;
                    n_data = curr_darr->data;
                }
            }
        }
    }
    return NULL;
}
