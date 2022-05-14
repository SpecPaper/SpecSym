/*
test16 (Kocher15)
Pass a pointer to the length
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


void victim_fun(size_t *idx) {
	int x = 0;
    uint8_t y;
    if (x < array1_size) {                  
        temp &= array2[array1[x] * 512];
    }

	x = *idx;
	if( x < array1_size){
		y = array1[x];
	}

}

int main(int argn, char* args[]) {
    size_t source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(&source);
    return 0;
}

