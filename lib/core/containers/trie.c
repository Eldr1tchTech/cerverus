#include "trie.h"

#include <string.h>

void trie_create(trie *t, size_t dat_size)
{
    t = (trie *)cmem_alloc(memory_tag_trie, sizeof(trie));
    t->data_size = dat_size;
    darray_create(t->children, sizeof(trie_node *), DEFAULT_CHILDREN_DARRAY_SIZE);
}

// make some sort of recursive function
void trie_destroy(trie *t);

trie_node *create_node(trie *t)
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
    for (int i = 0; i < strlen(n->key); i++)
    {
        for (int j = 0; j < curr_darr->length; j++)
        {
            if (strcmp(n->key[i], n_data[j]->key) == 0)
            {
                if (i == strlen(n->key) - 1)
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
                new_node->key = n->key[i];
                darray_add(n_data[j]->children, new_node);
                curr_darr = new_node->children;
                n_data = curr_darr->data;
            }
        }
    }
}

// returns the data, or NULL if it could not be found
void *trie_search(trie *t, char *k_arg)
{
    darray *curr_darr = t->children;
    trie_node **n_data = curr_darr->data;
    int on_track = 1;
    for (int i = 0; i < strlen(k_arg) && on_track; i++)
    {
        on_track = 0;
        for (int j = 0; j < curr_darr->length && !on_track; j++)
        {
            if (strcmp(k_arg[i], n_data[j]->key) == 0)
            {
                if (i == strlen(k_arg) - 1)
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

// The following aren't as important, so they have not been implemented yet.
void trie_delete(trie *t, char *k_arg);

int trie_contains(trie *t, char *k_arg);
