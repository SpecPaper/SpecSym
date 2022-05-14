/*
test12 (Kocher11)
Use memcmp() to read the memory for the leak.
*run "ulimit -s unlimited" before klee.
No BTB restrictions.
SEW=40
run 'ulimit -s unlimited' before execution
*/
#include <stdio.h>
#include <stdlib.h>
#include "../../../klee/include/klee/klee.h"
#include <stdint.h>
#include <string.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


void victim_fun(int idx) {
    if (idx < array1_size) {                  
        temp = memcmp(&temp, array2 + (array1[idx] * 512), 1);
    }
}

int main(int argn, char* args[]) {
    int source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}

