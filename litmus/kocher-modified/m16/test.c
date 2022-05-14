/*
test03
How out of place jumps can be harmful. KleeSpectre detects lines 44-45 as sensitive, however using BTB we detect a sensitive behaviour from lines 39,42,27. We can prevent such an attack by putting a fence at line 26.
BTB(set=1, tag=0, ways=1)
*/

#include <stdint.h>
#include "../../../klee/include/klee/klee.h"

unsigned int array1_size = 16; 
uint8_t array1[16];
uint8_t array2[256 * 64];
uint8_t temp = 0;


__attribute__ ((optnone))
uint8_t victim_fun(int idx)
{
    int y = 0;
    int b = 2*3;
    int a = 12;
    int temp;

    if (idx < a) {
        //fence
        //__asm__("MFENCE");
        temp &= array2[y*64];
    }

    //dummy
    b = 2;
    a = 2;
    b = a*123;
    a = b*321;
    b = a*123;
    a = b*321;
    int d = a+b;
    
    y = array1[idx]; // loading secret into y.
    

    if (idx < array1_size) { // After reaching this branch, we execute line 27 speculatively. 
        //some dummy instructions
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

