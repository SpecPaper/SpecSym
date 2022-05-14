/*
test04 (Kocher03)
Moving the leak to a local no-inlined function.
No BTB restrictions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../../../klee/include/klee/klee.h"

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;

struct timespec time_start, time_end;
size_t time_diff;



static __attribute__ ((noinline)) void leakByteNoinlineFunction(uint8_t k) { 
    temp &= array2[k* 512]; 
}

void victim_fun(size_t idx) {
	int x = 0;
	uint8_t y;    
	if (x < array1_size)
        leakByteNoinlineFunction(array1[x]);

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

