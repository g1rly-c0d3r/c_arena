/* Author: eos Shapland
 * 22-10-2025
 *
 * This example demonstrates how to define an arena with a set size,
 * and use that arena as a stack.
 *
 * Compiled with:
 * clang -o example example.c -larena
 */

#include <arena.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <error.h>

void *thread_fn(void *arga);

int main(int ac, char **av){
    if (ac<2) {
        error(1, 0, "Usage: ./example <arena_size>");
    }

    // Create an arena with user-specified capacity
    arena_t *main_arena = arena_create_with_capacity(strtoull(av[1], NULL, 10));

    printf("main arena capacity: %lu\n", main_arena->capacity);


    // we can define an array of arenas
    // creates 4 arenas for each thread
    // instead of creating a thread args struct, we can pass an arena as a void*
    arena_t *thread_arenas[] = { arena_create(), arena_create(), arena_create(), arena_create() };
    pthread_t threads[4];

    // the for loop just spawns 4 threads
    for(uint8_t i=0; i<4; i++){
        // We want to pass the string im msg as well as the value of i to each thread
        // to do this, we will make pointers that point to a spot on the arena, 
        // then pass the arena itself to the thread. 
        // THIS IS NOT THREAD-SAFE.
        // if you were write to these pointers, 
        // there would be no guarantee that there wouldn't be weird side-effects
        //
        // this model of passing arguments to threads consists of 3 parts:
        // one integer that is the length of the second part
        // an array of integers that keeps track of the offsets of the actual arguments to the thread
        // the arguments themselves
        //
        // This is the only way I thought of to make sure none of the data gets read wrong
        // this method is not part of the arena library, but if you are looking for a way to pass
        // arguments to threads and you don't want to define a new struct for every thread function
        // then this works.
        
        // the first allocation is the number of arguments we want to send to the thread
        uint64_t *arrlen = arena_push(thread_arenas[i], sizeof(uint64_t));
        *arrlen = 1;
        // then we want to keep track of the offsets of each argument
        // (there is an implied offset of (1+*arrlen)*sizeof(uint64_t),
        // which is the number of args plus their offsets)
        uint64_t *arr = arena_push(thread_arenas[i], *arrlen * sizeof(uint64_t));
        uint64_t msglength = 64;
        arr[0] = msglength;
        // Now, we can allocate the actuall arguments themselves
        char *msg = arena_push(thread_arenas[i], msglength);
        uint64_t *iptr = arena_push(thread_arenas[i], sizeof(uint64_t));

        *iptr = i;
        strncpy(msg, "hello from thread %d", msglength);

        pthread_create(&threads[i],
                       NULL,
                       thread_fn,
                       // now we can cast the arena to void * and pass it to the thread.
                       (void*)thread_arenas[i]
                      );
    }


    for (uint8_t i; i<4; i++){
        pthread_join(threads[i], NULL);
    
    }

    // only need to de-allocate the arena instances, instead of everything stored on them.
    arena_free(main_arena);
    return 0;
}

void *thread_fn(void *arga){
    // take the arguments and cast them to an arena ptr
    arena_t *scratch = (arena_t*)arga;
    // get the length of the array of indices,
    // and get the offset of the array itself
    uint64_t arrlen = *(uint64_t *)scratch->elem;
    uint64_t *arr = (uint64_t *)scratch->elem + 1;
    // after this point, it is up to you to make sure the data gets cast to the correct type
    char *msg = (char *)scratch->elem + (1+arrlen)*sizeof(uint64_t);
    uint64_t i = *((uint64_t *)scratch->elem+(1+arrlen)+arr[0]/sizeof(uint64_t));

    // now we can use the arena like the runtime stack
    char *fmt = arena_push(scratch, arr[0]);

    snprintf(fmt, arr[0], msg, i);

    printf("%s\n", fmt);

    // do some work possibly

    // if you use some memory, you still have to remember to pop it off at some point
    // if you choose, you can omit this and once the thread has returned, clear the arena.
    arena_pop(scratch, arr[0]);
    return (void*)0;
}
