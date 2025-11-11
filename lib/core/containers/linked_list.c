#include "linked_list.h"

#include "core/logger.h"

linked_list *linked_list_create(size_t data_size)
{
    linked_list *ll = (linked_list *)cmem_alloc(memory_tag_linked_list, sizeof(linked_list));
    ll->data_size = data_size;
    ll->root = NULL;
}

void linked_list_destroy(linked_list *ll)
{
    if (!ll)
    {
        LOG_ERROR("linked_list_destroy - linked list was never initialized.");
        return;
    }

    linked_list_node *curr_node;
    linked_list_node *temp;
    curr_node = ll->root;
    while (curr_node)
    {
        temp = curr_node;
        curr_node = curr_node->next;
        Linked_list_node_destroy(ll, temp);
    }

    ll->data_size = 0;
    // ll->root is already destroyed.
    cmem_free(memory_tag_linked_list, ll, sizeof(linked_list));
    ll = NULL;
}

linked_list_node *linked_list_node_create(linked_list *ll)
{
    linked_list_node *ll_node = (linked_list_node *)cmem_alloc(memory_tag_linked_list, sizeof(linked_list_node));

    ll_node->prev = NULL;
    ll_node->next = NULL;
    ll_node->data = cmem_alloc(memory_tag_linked_list, ll->data_size);

    return ll_node;
}

void Linked_list_node_destroy(linked_list *ll, linked_list_node *node)
{
    if (!node || !ll)
    {
        LOG_ERROR("Linked_list_node_destroy - The linked list and node must both exist.");
        return;
    }

    void *next = node->next;
    void *prev = node->prev;
    if (next && prev)
    {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    else if (next && !prev)
    {
        node->next->prev = NULL;
    }
    else if (!next && prev)
    {
        node->prev->next = NULL;
    }

    node->next = NULL;
    node->prev = NULL;
    cmem_free(memory_tag_linked_list, node->data, ll->data_size);
    node->data = NULL;
    cmem_free(memory_tag_linked_list, node, sizeof(linked_list_node));
    node = NULL;
}

// If prev_node is NULL it is inserted at the very beginning.
void linked_list_insert(linked_list *ll, linked_list_node *prev_node, linked_list_node *node)
{
    if (!node || !ll)
    {
        LOG_ERROR("linked_list_insert - The linked list and node must both exist.");
        return;
    }
    
    if (!prev_node)
    {
        linked_list_node* temp = ll->root;
        ll->root = node;
        node->next = temp;
        temp->prev = node;
        return;
    }
    
    node->prev = prev_node;
    node->next = prev_node->next;
    prev_node->next = node;
    if (node->next)
    {
        node->next->prev = node;
    }
}