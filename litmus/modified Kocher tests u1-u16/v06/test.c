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

unsigned int array1_size = 4;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


void victim_fun(int idx) {
    size_t i;
	int x = 5;
	uint8_t y;
    if (x < array1_size) {                  
        for (i = x; i >= 0; i--) 
            temp &= array2[array1[x] * 512];
    }

	x = idx;
	if( x < array1_size){
		y = array1[x];
	}

}

int main(int argn, char* args[]) {
    int source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}

