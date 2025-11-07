#include "route_trie.h"

#include "core/memory/cmemory.h"

#include <string.h>

void route_trie_node_destroy(route_trie_node *t_node)
{
    route_trie_node **node_arr = t_node->children->data;
    for (int i = 0; i < t_node->children->length; i++)
    {
        route_trie_node_destroy(node_arr[i]);
    }

    darray_destroy(t_node->children);
    cmem_free(memory_tag_route_trie, t_node, sizeof(route_trie_node));
}

int char_count(char *str, char c)
{
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == c)
            count++;
    }
    return count;
}

void path_parser(darray *darr, char *path)
{
    if (path[strlen(path) - 1] == '/')
    {
        path[strlen(path) - 1] = '\0';
    }

    // 0 length should be checked before this.
    if (strlen(path) == 0)
        return;

    int new_length = char_count(path, '/');
    if (new_length == 0)
    {
        LOG_ERROR("path_parser - Could not correctly parse the path.");
        return;
    }

    if (darr->darr_size < new_length)
    {
        darray_resize(darr, new_length);
        darr->length = new_length;
    }

    strtok(path, "/");
    char **darr_data = (char *)darr->data;
    for (int i = 0; i < new_length - 1; i++)
    {
        darr_data[i] = strtok(NULL, "/");
    }
}

route_trie_node *route_trie_find_node(route_trie *r_trie, http_method method, char *path)
{
    darray *temp_darr;
    darray_create(temp_darr, sizeof(char *), DARRAY_DEFAULT_SIZE);
    path_parser(temp_darr, path);

    route_trie_node* result = NULL;
    darray *branch = r_trie->children[method];
    route_trie_node** branch_data;
    char** path_data = temp_darr->data;
    int on_track = 1;
    for (int j = 0; j < temp_darr->length && on_track; j++)
    {
        on_track = 0;
        branch_data = branch->data;
        for (int i = 0; i < branch->length; i++)
        {
            if (strcmp(path_data[j], branch_data[i]->segment) == 0 || branch_data[i]->type == route_segment_type_dynamic)
            {
                if (j == temp_darr->length - 1)
                {
                    return result = branch_data[i];
                } else {
                    branch = branch_data[i]->children;
                }
                on_track = 1;
                break;
            }
        }
    }

    // If it ever reaches this point it would return NULL
    return result;
}

void route_trie_create(route_trie *r_trie)
{
    r_trie = cmem_alloc(memory_tag_route_trie, sizeof(route_trie));
    for (int i = 0; i < http_method_unknown; i++)
    {
        darray_create(r_trie->children[i], sizeof(route_trie_node *), DARRAY_DEFAULT_SIZE);
    }
}

void route_trie_destroy(route_trie *r_trie)
{
    for (int i = 0; i < http_method_unknown; i++)
    {
        route_trie_node **node_arr = r_trie->children[i]->data;
        for (int j = 0; j < r_trie->children[i]->length; j++)
        {
            route_trie_node_destroy(node_arr[j]);
        }

        darray_destroy(r_trie->children[i]);
    }

    cmem_free(memory_tag_route_trie, r_trie, sizeof(route_trie));
}

route_handler *route_trie_find_handler(route_trie *r_trie, http_method method, char *path)
{
    return route_trie_find_node(r_trie, method, path)->route_handler;
}

void route_trie_add_route(route_trie *r_trie, http_method method, char *path, route_handler *handler)
{
    
}