#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void enqueue(struct queue *q, struct game_state state) {
    uint64_t serialized = serialize(state);
    insert_at_tail(&(q->data), (size_t)serialized);
}

struct game_state dequeue(struct queue *q) {
    size_t serialized = remove_from_head(&(q->data));
    return deserialize((uint64_t)serialized);
}

#define HASH_SIZE 100003

typedef struct visited_node {
    uint64_t key;
    struct visited_node *next;
} visited_node;

static visited_node *visited[HASH_SIZE];

static int hash_key(uint64_t key) {
    return key % HASH_SIZE;
}

static bool visited_contains(uint64_t key) {
    int h = hash_key(key);
    visited_node *node = visited[h];
    while (node) {
        if (node->key == key)
            return true;
        node = node->next;
    }
    return false;
}

static void visited_insert(uint64_t key) {
    int h = hash_key(key);
    visited_node *node = malloc(sizeof(visited_node));
    if (!node)
        exit(1); 
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
                if (state.tiles[row][col] != 0)
                    return false;
            } else {
                if (state.tiles[row][col] != expected)
                    return false;
                expected++;
            }
        }
    }
    return true;
}

static void free_queue(struct queue *q) {
    free_list(q->data);
    q->data.head = NULL;
}

int number_of_moves(struct game_state start) {
    if (is_solved(start))
        return start.num_steps;

    struct queue q;
    q.data.head = NULL;

    for (int i = 0; i < HASH_SIZE; i++) {
        visited[i] = NULL;
    }
    uint64_t start_ser = serialize(start);
    visited_insert(start_ser);
    enqueue(&q, start);

    while (q.data.head != NULL) {
        struct game_state cur = dequeue(&q);
        if (is_solved(cur)) {
            free_visited();
            free_queue(&q);
            return cur.num_steps;
        }
        uint64_t cur_ser = serialize(cur);
        struct game_state next;
        uint64_t next_ser;

        next = cur;
        move_up(&next);
        next_ser = serialize(next);
        if (next_ser != cur_ser && !visited_contains(next_ser)) {
            visited_insert(next_ser);
            enqueue(&q, next);
        }

        next = cur;
        move_down(&next);
        next_ser = serialize(next);
        if (next_ser != cur_ser && !visited_contains(next_ser)) {
            visited_insert(next_ser);
            enqueue(&q, next);
        }

        next = cur;
        move_left(&next);
        next_ser = serialize(next);
        if (next_ser != cur_ser && !visited_contains(next_ser)) {
            visited_insert(next_ser);
            enqueue(&q, next);
        }

        next = cur;
        move_right(&next);
        next_ser = serialize(next);
        if (next_ser != cur_ser && !visited_contains(next_ser)) {
            visited_insert(next_ser);
            enqueue(&q, next);
        }
    }
    free_visited();
    return -1;
