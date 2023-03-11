#include "linked_list.h"

Node* new_node(void* elem, Node* next)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->elem = elem;
    node->next = next;
    return node;
}

// Recursively frees a linked list of nodes.
void free_linked_nodes(Node* node, void (*free_func_ptr)(void*))
{
    if (*free_func_ptr != NULL)
        (*free_func_ptr)(node->elem);

    if (node->next != NULL)
        free_linked_nodes(node->next, *free_func_ptr);

    free(node);
}

LinkedList* new_linked_list(void* elem)
{
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));

    list->first = (elem != NULL) ? new_node(elem, NULL) : NULL;
    list->last = list->first;
    list->num_elems = (list->first == NULL) ? 0 : 1; 
    return list;
}

void free_linked_list(LinkedList* list, void (*free_func_ptr)(void*))
{
    if (list->first != NULL)
        free_linked_nodes(list->first, *free_func_ptr);
        
    free(list);
}

void add_linked_list_elem(LinkedList* list, void* elem)
{
    Node* node = new_node(elem, NULL);

    if (list->first == NULL)
    {
        list->first = node;
        list->last = node;
    }
    else
    {
        list->last->next = node;
        list->last = node;
    }

    list->num_elems++;
}
