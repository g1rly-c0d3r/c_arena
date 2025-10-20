#ifndef ARENA
#define ARENA

#include <stdint.h>

typedef struct{
    uint64_t capacity;
    uint64_t length;
    void *elem;
}arena_t;

// initalizes an arena with the default capacity (100 64-bit values).
// Assumes the arena is not pre-allocated
arena_t *arena_create();
// initializes an arena with a user-defined capacity.
// Assumes the arena is not pre-allocated
void arena_init_with_capacity(arena_t *arena, uint64_t capacity);

// re-sizes the arena to the desired capacity.
// if bytes < arena->length, then the arena is truncated to fit.
void arena_realloc(arena_t *arena, uint64_t bytes);

// "allocate" something, specifying the number of bytes to allocate
// returns a pointer to the valid data space
void *arena_push(arena_t *arena, uint64_t numBytes);

// free some bytes off of the top of the arena
void arena_pop(arena_t *arena, uint64_t numBytes);

// clear arena whithout any free'ing
void arena_clear(arena_t *arena);
// de-allocate an arena, and free all of the used memory
void arena_free(arena_t *arena);

#endif
