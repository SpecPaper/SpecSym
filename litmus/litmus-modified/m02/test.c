/*
test02
This example shows how kleespectre can be wrong. Kleespectre marks lines 24-25 as an attack, but using BPL we eliminate that possibility.
No BTB restrictions.
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
    int b = 2*3;
    int a = 12;
	int x = 0;
    if (x >= array1_size) {    
	    b += 5*55*555*5555;
	    b = a*32;
    } else {
		y = array1[x];
		temp &= array2[y*64];
    }

    /* These two lines disable the compiler optimization of array */
    array2[0] = 2;  
    array1[0] = 2;

	x = idx;
	if( x < array1_size){
		y = array1[x];
	}

    return temp;
}

int main() {
	int i, source;
    klee_make_symbolic(&source, sizeof(source), "source");
	victim_fun(source);
    return 0;
}

