/*
test06 (Kocher05)
Use x as the initial value in a for() loop.
No BTB restrictions.
compile "clang-6.0 -g -c -emit-llvm test.c -o test.bc"
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../../klee/include/klee/klee.h"

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


void victim_fun(int idx) {
    size_t i;
    if (idx < array1_size) {                  
        for (i = idx; i >= 0; i--)
            temp &= array2[array1[i] * 512];
    }
}

int main(int argn, char* args[]) {
    int source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}

