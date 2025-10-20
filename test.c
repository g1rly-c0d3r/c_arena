#include <stdint.h>
#include "arena.h"


int main(int ac, char** av) {
    arena_t *arena = arena_create();


    arena_free(arena);
    return 0;
}
