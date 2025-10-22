## Getting Started

First, [install](../README.md#Installation) the library. 

Once `arena.h` has been installed, you can `#include <arena.h>` in your C or C++ code. 

For examples, check the [examples](./examples) directory.

## The `arena_t` type
`arena_t` is the type name of an arena, which is just a pre-allocated chunk of memory.
Each arena keeps track of its capacity, as well as the current position of the next available memory address.

An arena behaves like a stack where pushing some bytes will increment the length, and poping some bytes will decriment the length.
If you try to push more bytes than the arena curently has room for, `realloc` is called to re-size the arena
to 1.5 times the number of bytes you want to push. 
It is recomended to pre-allocate your arena with `arena_create_with_capacity` to minimize re-allocation.
`arena_create` will allocate space for 100 64-bit values, for when you do not have an idea of the size of your data.

To allocate some data on the arena, call `arena_push` like you would [`malloc(3)`](https://man7.org/linux/man-pages/man3/malloc.3.html).
From there, you are free to use that data just like any other variable.
Once you are done with your ***most recent*** data, call `arena_pop` like you would pop a value off of a stack,
specifying the number of bytes to pop off. There is no way to remove data from the middle of an arena, that is beyond the scope of this project and is left to the user.

To clear an arena without de-allocating it, simply call `arena_clear`. To de-allocate an arena ([`free(3)`](https://man7.org/linux/man-pages/man3/malloc.3.html) it),
call `arena_free`.

### Notes on `arena_realloc`
If you must re-allocate the arena yourself, note that you will need to store the position in the arena of any allocated variables,
and restore the pointer to valid data after you call `arena_realloc`, like so:
```C
uint64_t arrpos = arena->length;
int *arr = (int *)arena_push(12*sizeof(int));

// do stuff

arena_realloc(N);
arr = (char *)arena->elem + arrpos;
```
Casting to `char *` is done to do pointer arithmetic, and `sizeof(char) == 1`. 
This is a necessary because [`realloc(3)`](https://man7.org/linux/man-pages/man3/realloc.3.html) may not return the same block of memory,
and `arr` still points to the old block.

`arena_push` will call `arena_realloc` if you request to push passed the capacity of the arena, so it is recommended to pre-allocate a large enough arena to prevent needing to re-assign all allocated variables.
