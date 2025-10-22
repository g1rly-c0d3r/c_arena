#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "arena.h" // current dir version of arena.h

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// if you add tests, you will need to update this number
#define NUM_TESTS 6

// Declare tests here
void create_test();
void create_w_cap_test();
void realloc_test();
void push_test();
void pop_test();
void clear_test();


void run_tests(char** tests, int numtest);

// If you add tests, you will need to add them to this function pointer array
void (*test_fns[NUM_TESTS])() = {
    create_test,
    create_w_cap_test,
    realloc_test,
    push_test,
    pop_test,
    clear_test,
};

// tests if argv has a valid list of tests, 
// or if we want to run all of the tests
bool valid(char **tests, int numtest){
    if(strcmp(tests[1], "all") == 0){
        return true;
    }

    for(int i = 1; i < numtest; i++){
        if (strtoul(tests[i], NULL, 10) == 0 || strtoul(tests[i], NULL, 10) > NUM_TESTS) {
            fprintf(stderr, "Tests range from 1-%d\n", NUM_TESTS);
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2 || !valid(argv, argc) ){
        fprintf(stderr, "Usage: ./test < all | test_num_list>\n");
        exit(1);
    }
    run_tests(argv, argc);

    return 0;
}

void create_test(){
    arena_t *arena = arena_create();
    if (arena->capacity == 800 && arena->length == 0){
        printf("Test 1 %spassed%s! (%screate%s)\n", KGRN , KNRM, KYEL, KNRM);
    } else{
        printf("Test 1 %sFailed%s! (%screate%s)\n", KRED, KNRM, KYEL, KNRM);
    }
    arena_free(arena);
}

void create_w_cap_test(){
    arena_t *arena = arena_create_with_capacity(100);
    if(arena->capacity == 100 && arena->length==0){
        printf("Test 2 %spassed%s! (%screate_w_cap%s)\n", KGRN , KNRM, KYEL, KNRM);
    } else {
        printf("Test 2 %sFailed%s! (%screate_w_cap%s)\n",KRED,KNRM, KYEL, KNRM);
    }
    arena_free(arena);
}

void realloc_test(){
    arena_t *arena = arena_create();

    // strpos must be used after a realloc 
    // to ensure that str still points to valid data.
    uint64_t strpos = arena->length;
    char *str = arena_push(arena, 7);

    strncpy(str, "hello", 7);

    arena_realloc(arena, 10);
    str = (char *)arena->elem + strpos;
    if (arena->capacity != 10 || strncmp(str, "hello", 7) != 0)
        goto fail;
    arena_realloc(arena, 1200);
    str = (char *)arena->elem + strpos;
    if(arena->capacity != 1200)
        goto fail;
    arena_realloc(arena, 3); // after this, str will only be valid for 3 bytes
    str = (char *)arena->elem + strpos; 
    // reading past 3 bytes might segfault
    if(strncmp(str, "hel", 3) != 0)
        goto fail;

    printf("Test 3 %spassed%s! (%srealloc%s)\n", KGRN, KNRM, KYEL, KNRM);
    arena_free(arena);
    return;
fail:
    printf("Test 3 %sFailed%s! (%srealloc%s)\n", KRED, KNRM, KYEL, KNRM);
    arena_free(arena);
    return;
}

void push_test(){
    arena_t *arena = arena_create_with_capacity(10);

    uint64_t str1pos = arena->length;
    char *str1 = arena_push(arena, 5);
    if(arena->length != 5)
        goto fail;

    strncpy(str1, "hai", 5);
    if(strncmp(str1, "hai", 5) != 0)
        goto fail;

    char *str2 = arena_push(arena, 7); // since we are pushing past arena->length, realloc is called.
                                       // so anything else on this arena need to be restored.
    strncpy(str2, "hello", 7);
    str1 = (char *)arena->elem + str1pos;
    if(arena->length != 12 || strncmp(str2, "hello", 7) != 0 || strncmp(str1, "hai", 5))
        goto fail;

    printf("Test 4 %spassed%s! (%spush%s)\n", KGRN, KNRM, KYEL, KNRM);
    arena_free(arena);
    return;
fail:
    printf("Test 4 %sFailed%s! (%spush%s)\n", KRED, KNRM, KYEL, KNRM);
    arena_free(arena);
    return;
}

void pop_test(){
    arena_t *arena = arena_create();

    int *arr = arena_push(arena, 8*sizeof(int));

    for(int i = 0; i < 8; i++){
        arr[i] = i;
    }

    arena_pop(arena, 4*sizeof(int));

    if(arena->length != 4*sizeof(int))
        goto fail;

    printf("Test 5 %spassed%s! (%spop%s)\n", KGRN, KNRM, KYEL, KNRM);
    arena_free(arena);
    return;
fail:
    printf("Test 5 %sFailed%s! (%spop%s)\n", KRED, KNRM, KYEL, KNRM);
    arena_free(arena);
    return;
}

void clear_test(){
    arena_t *arena = arena_create();

    int *arr = arena_push(arena, 12*sizeof(int));

    arr[4] = 6;

    arena_clear(arena); // arr unusable after this point

    if(arena->length != 0)
        goto fail;

    printf("Test 6 %spassed%s! (%sclear%s)\n", KGRN, KNRM, KYEL, KNRM);
    arena_free(arena);
    return;
fail:
    printf("Test 6 %sFailed%s! (%sclear%s)\n", KRED, KNRM, KYEL, KNRM);
    arena_free(arena);
    return;
}

void run_tests(char **tests, int numtest){
    if (strcmp(tests[1],  "all") == 0){
        for (uint8_t i = 0; i < NUM_TESTS; i++){
            test_fns[i]();
        }
    } else {
        for(uint8_t i = 1; i < numtest; i++){
            test_fns[strtoul(tests[i],NULL, 10)-1]();
        }
    }
}
