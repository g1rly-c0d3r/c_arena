#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include "arena.h"

#define NUM_TESTS 0

void (*test_fns[NUM_TESTS])();

void run_tests(char** tests);


int main(int ac, char** av) {
    run_tests(av);

    return 0;
}

void run_tests(char **tests){
    if (strcasecmp(tests[1],  "all")){
        for (uint8_t i = 0; i < NUM_TESTS; i++){
            test_fns[i]();
        }
    }
    
        
}
