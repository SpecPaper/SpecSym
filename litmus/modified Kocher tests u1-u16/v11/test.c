/*
test11 (Kocher10)
Leak a comparison result. control dependency
No BTB restrictions.
*/

#include <stdio.h>
#include "../../../klee/include/klee/klee.h"
#include <stdlib.h>
#include <stdint.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


void victim_fun(int idx, int k) {
	int x = 0;
    uint8_t y;
    if (x < array1_size) {                  
        if (array1[x] == k)
            temp &= array2[0];
    }

	x = idx;
	if( x < array1_size){
		y = array1[x];
	}

}

int main(int argn, char* args[]) {
    int source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source, 10);
    return 0;
}

