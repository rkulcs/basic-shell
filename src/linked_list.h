#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node
{
    void* elem;
    Node* next;
};

typedef struct LinkedList
{
    int num_elems;
    Node* first;
    Node* last;
} LinkedList;

Node* new_node(void* elem, Node* next);
void free_linked_nodes(Node* node, void (*free_func_ptr)(void*));

LinkedList* new_linked_list(void* elem);
void add_linked_list_elem(LinkedList* list, void* elem);
void free_linked_list(LinkedList* list, void (*free_func_ptr)(void*));
