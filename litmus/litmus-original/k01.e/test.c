/*
test02
*/

#include <stdint.h>
#include "../../../klee/include/klee/klee.h"

unsigned int array1_size = 16; 
uint8_t array1[16];
uint8_t array2[256 * 64];
uint8_t temp = 0;


uint8_t victim_fun(int idx)  __attribute__ ((optnone)) 
{
    int y = 0;
    int b = 6;
    int a = 12;
    if (idx >= array1_size) {
		//dummy computation.
		y += (a*b) + (a/b);
		y += a/b;
    } else {
		y = array1[idx];
		temp &= array2[y*64];
    }

    /* These two lines disable the compiler optimization of array */
    array2[0] = 2;  
    array1[0] = 2;
    return temp;
}

int main() {
	int i, source;
    klee_make_symbolic(&source, sizeof(source), "source");
	victim_fun(source);
    return 0;
}

