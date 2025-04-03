#include "linked_list.h"
#include <stdlib.h>

// Create a new list node with the given value.
struct list_node *new_node(size_t value) {
    struct list_node *node = malloc(sizeof(struct list_node));
    if (node != NULL) {
        node->value = value;
        node->next = NULL;
    }
    return node;
}

// Insert a new node at the head of the linked list.
void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (node == NULL) return;  // allocation failure
    node->next = list->head;
    list->head = node;
}

// Insert a new node at the tail of the linked list.
void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (node == NULL) return;  // allocation failure
    if (list->head == NULL) {
        // List is empty: new node becomes the head.
        list->head = node;
    } else {
        struct list_node *cur = list->head;
        // Traverse to the last node.
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = node;
    }
}

// Remove a node from the head of the list and return its value.
size_t remove_from_head(struct linked_list *list) {
    if (list->head == NULL) {
        // List is empty; behavior is undefined.
        return 0;
    }
    struct list_node *node = list->head;
    size_t value = node->value;
    list->head = node->next;
    free(node);
    return value;
}

// Remove a node from the tail of the list and return its value.
size_t remove_from_tail(struct linked_list *list) {
    if (list->head == NULL) {
        // List is empty.
        return 0;
    }
    // If there's only one element.
    if (list->head->next == NULL) {
        size_t value = list->head->value;
        free(list->head);
        list->head = NULL;
        return value;
    }
    // Traverse to find the node before the last one.
    struct list_node *prev = NULL;
    struct list_node *cur = list->head;
    while (cur->next != NULL) {
        prev = cur;
        cur = cur->next;
    }
    size_t value = cur->value;
    prev->next = NULL;
    free(cur);
    return value;
}

// Free all nodes in the list.
void free_list(struct linked_list list) {
    struct list_node *cur = list.head;
    while (cur != NULL) {
        struct list_node *next = cur->next;
        free(cur);
        cur = next;
    }
}

// Utility function to help with debugging; do not modify.
void dump_list(FILE *fp, struct linked_list list) {
    fprintf(fp, "[ ");
    for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
        fprintf(fp, "%zu ", cur->value);
    }
    fprintf(fp, "]\n");
}
