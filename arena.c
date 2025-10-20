#include "arena.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

arena_t *arena_create(){
    arena_t *arena = (arena_t *) malloc(sizeof(arena_t));
    // magic number for now, will do some tuning later
    arena->capacity = 100*sizeof(uint64_t);
    arena->elem = malloc(arena->capacity);
    arena->length = 0;

    return arena;
}

arena_t *arena_create_with_capacity(uint64_t capacity){
    arena_t *arena;
    arena = (arena_t *) malloc(sizeof(arena_t));

    arena->capacity = capacity;
    arena->elem = malloc(arena->capacity);
    arena->length = 0;

    return arena;
}

void arena_realloc(arena_t *arena, uint64_t bytes){
    void *new_elem = malloc(bytes);

    if (arena->length < bytes)
        memcpy(new_elem, arena->elem,  bytes);
    else
        memcpy(new_elem, arena->elem, arena->length);

    free(arena->elem);
    arena->elem = new_elem;
}

void *arena_push(arena_t *arena, uint64_t numBytes){
    void *ptr;
    // if the arena is not large enough, then we need to re-size it
    if(arena->capacity < arena->length + numBytes){
        arena_realloc(arena, ((uint64_t)1.5*numBytes) + arena->capacity);
    }

    // resulting memory is the arena offset plus the number of bytes currently allocated
    ptr = arena + arena->length;

    arena->length += numBytes;

    return ptr;
}

void arena_pop(arena_t *arena, uint64_t numBytes){
    arena->length -= numBytes;
}


void arena_free(arena_t *arena){
    free(arena->elem);
    free(arena);
}
