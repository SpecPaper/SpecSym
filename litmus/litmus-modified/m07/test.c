/*
test07 (Kocher06)
Check the bounds with an AND mask, rather than "<"
No BTB restrictions.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../../klee/include/klee/klee.h"

unsigned int array1_size = 16;
unsigned int array_size_mask = 15;
uint8_t array1[16];
uint8_t array2[256*512];
uint8_t temp = 0;

void victim_fun(int idx) {
	int x = 0;
	uint8_t y;
    if ((x & array_size_mask) == x) {                  
        temp &= array2[array1[x]];
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


/*
In this example idx is assumed to be inBound, so array1[idx] doesn't make a sensitive expression.
*/
