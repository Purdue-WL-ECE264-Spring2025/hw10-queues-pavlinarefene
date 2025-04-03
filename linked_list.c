#include "linked_list.h"
#include <stdlib.h>

struct list_node *new_node(size_t value) {
  struct list_node *node = malloc(sizeof(struct list_node));
  if (node != NULL) {
    node->value = value;
    node->next = NULL;
  }
  return node;
}

void insert_at_head(struct linked_list *list, size_t value) {
  struct list_node *node = new_node(value);
  if (node == NULL) return; 
  node->next = list->head;
  list->head = node;
  if (list->tail == NULL) { 
    list->tail = node;
  }
}

void insert_at_tail(struct linked_list *list, size_t value) {
  struct list_node *node = new_node(value);
  if (node == NULL) return;
  if (list->tail == NULL) {
    list->head = node;
    list->tail = node;
  } else {
    list->tail->next = node;
    list->tail = node;
  }
}

size_t remove_from_head(struct linked_list *list) {
  if (list->head == NULL) {
    return 0;
  }
  struct list_node *node = list->head;
  size_t value = node->value;
  list->head = node->next;
  if (list->head == NULL) {
    list->tail = NULL;
  }
  free(node);
  return value;
}

size_t remove_from_tail(struct linked_list *list) {
  if (list->head == NULL) {
    return 0;
  }
  struct list_node *cur = list->head;
  struct list_node *prev = NULL;
  while (cur->next != NULL) {
    prev = cur;
    cur = cur->next;
  }
  size_t value = cur->value;
  if (prev == NULL) {
    list->head = NULL;
    list->tail = NULL;
  } else {
    prev->next = NULL;
    list->tail = prev;
  }
  free(cur);
  return value;
}

void free_list(struct linked_list list) {
  struct list_node *cur = list.head;
  while (cur != NULL) {
    struct list_node *next = cur->next;
    free(cur);
    cur = next;
  }
}

void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
