/*
The mixed example.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../../../klee/include/klee/klee.h"

unsigned int array1_size = 16; 
uint8_t array1[16] = {0};
uint8_t array2[256 * 64];

uint8_t marked_secret_array1[16];

unsigned int input_bound = 128;

// variables used
uint8_t k = 10;
uint8_t temp = 0;
unsigned int array_size_mask = 15;


uint8_t victim_fun(int idx)  __attribute__ ((optnone)) 
{
	int i;
	int y = 0;
    int b = 6;
    int a = 12;
	size_t j;
	static size_t last_x = 0;
	int* id = &idx;
    if(idx < input_bound){
        for(i = idx; i >= 0; i--){
            if(i == 0){
                //victim_fun0(idx);
                if (idx < array1_size) {    
                    y = array1[idx];
                    temp &= array2[y*64];
                }
            } else if(i == 1){
                //victim_fun1(idx);
                if (idx >= array1_size) {
                    //dummy computation.
                    y += (a*b) + (a/b);
                    y += a/b;
                } else {
                    y = array1[idx];
                    temp &= array2[y*64];
                }
            } else if(i == 4){
                //victim_fun4(idx);
                if (idx < array1_size) {                  
                    temp &= array2[array1[idx << 1] * 512];
                }
            } else if(i == 5){
                //victim_fun5(idx);
                if (idx < array1_size) {                  
                    for (j = idx; j >= 0; j--)
                        temp &= array2[array1[j] * 512];
                }
            } else if(i == 6){
                //victim_fun6(idx);
                if ((idx & array_size_mask) == idx) {                  
                    temp &= array2[array1[idx]];
                }
            } else if(i == 7){
                //victim_fun7(idx);
                if (idx == last_x) {                  
                    temp &= array2[array1[idx] * 512];
                }

                if (idx < array1_size)
                    last_x = idx;
            } else if(i == 8){
                //victim_fun8(idx);
                temp &= array2[array1[idx < (array1_size-1)? (idx + 1) : 0] * 512];
            } else if(i == 9){
                //victim_fun9(idx);
                if (idx < array1_size) {                 
                    if (array1[idx] == k)
                        temp &= array2[0];
                }
            } else if(i == 10){
                //victim_fun10(idx);
                if (idx < array1_size) {                  
                    y = array1[idx];
                    temp &= array2[y*64];
                }
            } else if(i == 12){
                //victim_fun12(idx);
                if (idx < array1_size) {                  
                    temp &= array2[array1[idx ^ 255] * 512];
                }
            } else{
                //victim_fun13(&idx);
                if (*id < array1_size) {                  
                    temp &= array2[array1[*id] * 512];
                }
            }     	
        }
    }

    array2[0] = 2;  
    array1[0] = 2;
    return temp;
}

int main() {
    int source;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}

