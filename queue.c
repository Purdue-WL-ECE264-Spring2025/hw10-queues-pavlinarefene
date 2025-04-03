#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void enqueue(struct queue *q, struct game_state state) {
  uint64_t serialized = serialize(state);
  insert_at_tail(&(q->list), (size_t)serialized);
}

struct game_state dequeue(struct queue *q) {
  size_t serialized = remove_from_head(&(q->list));
  return deserialize((uint64_t)serialized);
}

#define HASH_SIZE 100003

typedef struct visited_node {
  uint64_t key;
  struct visited_node *next;
} visited_node;

static visited_node *visited[HASH_SIZE];

// Simple hash function: use modulo.
static int hash_key(uint64_t key) {
  return key % HASH_SIZE;
}

static bool visited_contains(uint64_t key) {
  int h = hash_key(key);
  visited_node *node = visited[h];
  while (node) {
    if (node->key == key) return true;
    node = node->next;
  }
  return false;
}

static void visited_insert(uint64_t key) {
  int h = hash_key(key);
  visited_node *node = malloc(sizeof(visited_node));
  if (!node) exit(1); 
  node->key = key;
  node->next = visited[h];
  visited[h] = node;
}

static void free_visited() {
  for (int i = 0; i < HASH_SIZE; i++) {
    visited_node *node = visited[i];
    while (node) {
      visited_node *next = node->next;
      free(node);
      node = next;
    }
    visited[i] = NULL;
  }
}

static bool is_solved(struct game_state state) {
  int expected = 1;
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      if (row == 3 && col == 3) {
        if (state.tiles[row][col] != 0) return false;
      } else {
        if (state.tiles[row][col] != expected) return false;
        expected++;
      }
    }
  }
  return true;
}

int number_of_moves(struct game_state start) {
  if (is_solved(start)) return start.num_steps;

  struct queue q;
  q.list.head = NULL;
  q.list.tail = NULL;
  enqueue(&q, start);

  for (int i = 0; i < HASH_SIZE; i++) {
    visited[i] = NULL;
  }
  visited_insert(serialize(start));

  while (q.list.head != NULL) {
    struct game_state cur = dequeue(&q);
    if (is_solved(cur)) {
      free_visited();
      free_list(q.list);
      return cur.num_steps;
    }

    struct game_state next_state;
    uint64_t serialized_cur = serialize(cur);
    uint64_t serialized_next;

    next_state = cur;
    move_up(&next_state);
    serialized_next = serialize(next_state);
    if (serialized_next != serialized_cur && !visited_contains(serialized_next)) {
      visited_insert(serialized_next);
      enqueue(&q, next_state);
    }

    next_state = cur;
    move_down(&next_state);
    serialized_next = serialize(next_state);
    if (serialized_next != serialized_cur && !visited_contains(serialized_next)) {
      visited_insert(serialized_next);
      enqueue(&q, next_state);
    }

    next_state = cur;
    move_left(&next_state);
    serialized_next = serialize(next_state);
    if (serialized_next != serialized_cur && !visited_contains(serialized_next)) {
      visited_insert(serialized_next);
      enqueue(&q, next_state);
    }

    next_state = cur;
    move_right(&next_state);
    serialized_next = serialize(next_state);
    if (serialized_next != serialized_cur && !visited_contains(serialized_next)) {
      visited_insert(serialized_next);
      enqueue(&q, next_state);
    }
  }
  free_visited();
  return -1;
}
