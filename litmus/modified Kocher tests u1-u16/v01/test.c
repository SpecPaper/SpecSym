/*
test01
The traditional Kocher example.
No BTB restriction.
*/

#include <stdint.h>
#include "../../../klee/include/klee/klee.h"

unsigned int array1_size = 16; 
uint8_t array1[16];
uint8_t array2[256 * 64];
uint8_t array3[256 * 64];
uint8_t temp = 0;


uint8_t victim_fun(int idx)  __attribute__ ((optnone)) 
{
    int y = 0, x = 0;
    if (x < array1_size) {    
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
    int source = 20;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}


