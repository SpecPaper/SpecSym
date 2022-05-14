/*
test08 (Kocher07)
Compare against the last known-good value.
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
    static size_t last_x = 0;
	int x = 0;
    if (idx == last_x) {                  
        temp &= array2[array1[x] * 512];
    }

	x = idx;

    if (idx < array1_size)
        last_x = idx;

}

int main(int argn, char* args[]) {
    int source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}

