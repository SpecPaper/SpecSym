/*
test15 (Kocher14)
Invert the low bits of x.
No BTB restrictions.
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
	int x = 0;
    uint8_t y;
    if (x < array1_size) {                  
        temp &= array2[array1[x ^ 255] * 512];
    }

	x = idx;
	if( x < array1_size){
		y = array1[x ^ 255];
	}
}

int main(int argn, char* args[]) {
    int source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}

