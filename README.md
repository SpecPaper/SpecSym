# SpecSym
SpecSym is a symbolic execution engine with speculation semantics and cache modelling. SpecSym built on top of the KLEE symbolic execution engine, can thus provide a testing engine to check for the data leakage through cache side channel as shown via Spectre attacks. 
- Our prediction logic can precisely model the speculative behavior of different systems which is a crucial factor in detecting spectre attacks.  
- Our pattern detection mechanism can provide useful information to identify the root cause of data leakage, which is crucial for implementing an efficient defense mechanism.  
- Our symbolic cache model can verify whether the sensitive data leakage due to speculative execution can be observed by an attacker at a given program point. Our 

## Environment setting up.
This tool is based on KLEE <2.3, which needs the support of LLVM-6.0.  
NOTE: Suggest to refer "https://klee.github.io/build-llvm60/" to install all dependencies.  


### Install all the dependencies of LLVM
```
$sudo apt-get install build-essential curl libcap-dev git python cmake libncurses5-dev python3-minimal python3-pip unzip libtcmalloc-minimal4 libgoogle-perftools-dev libsqlite3-dev doxygen bison flex libboost-all-dev perl minisat python-dev
$ pip3 install tabulate
```

### Install LLVM-6.0
```
$ sudo apt-get install clang-6.0 llvm-6.0 llvm-6.0-dev llvm-6.0-tools
```
At this phase, it might be needed to install llvm as well.

### Install STP:
```
$ git clone https://github.com/stp/stp.git
$ cd stp/
$ git checkout tags/2.3.3
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ sudo cmake --install .
 ```

### Install uClibc
```
$ git clone https://github.com/klee/klee-uclibc.git  
$ cd klee-uclibc  
$ git checkout tags/klee_uclibc_v1.2
$ ./configure --make-llvm-lib  
$ make -j2  
$ cd ..
```
if you have multiple clang installations, use the following code:
```
CC=/usr/bin/clang-6.0 ./configure --make-llvm-lib
```

## Build SpecSym.
```
$ git clone https://github.com/SpecPaper/SpecSym.git
$ cd SpecSym/klee/
$ mkdir build
$ cd build
$ cp ../buid.sh .  
  # (or build_debug.sh for debug version)
 ```
The content of build.sh
```
cmake \
  -DENABLE_SOLVER_STP=ON \
  -DENABLE_POSIX_RUNTIME=ON \
  -DENABLE_KLEE_UCLIBC=ON \
  -DKLEE_UCLIBC_PATH=/PATH/TO/ULIBC \
  -DLLVM_CONFIG_BINARY=/usr/lib/llvm-6.0/bin/llvm-config \
  -DLLVMCC=/usr/bin/clang-6.0 \
  -DLLVMCXX=/usr/bin/clang++-6.0 \
  -DCMAKE_BUILD_TYPE=Release \
  ..
```
Change "/PATH/TO/ULIBC" to your ulibc path.  
For different cmake build types you may find more information [here](https://klee.github.io/docs/developers-guide/#Run-time%20libraries)

```
$ ./buid.sh
$ make -j 10
```    

Now you can use the "klee" and "klee-stat" in build/bin/

## Options to enable speculative execution and cache modeling
```
$ /PATH/TO/KLEE/ROOT/klee --help
...
Speculative execution options:
These options impact the speculative paths exploring and the cache modeling

-BTB-sets=<uint>                                       - BTB sets (default=256)
-BTB-tag-bits=<uint>                                   - Number of BTB tag bits (default=20)
-BTB-ways=<uint>                                       - BTB ways (default=1)
-PHT-reg-size=<uint>                                   - PHT register size (default=8)
-cache-line-size=<uint>                                - Cache line size (default=64)
-cache-sets=<uint>                                     - Cache sets (default=256)
-cache-ways=<uint>                                     - Cache ways (default=2)
-enable-btbmodel                                       - Enable BTB modeling (default=off).
-enable-cachemodel                                     - Enable Cache modeling (default=off).
-enable-phtmodel                                       - Enable PHT modeling (default=off).
-enable-speculative                                    - Enable Speculative exeuction modeling (default=off).
-max-sew=<uint>                                        - Maximum SEW (default=10)
-only-detect-marked-secret                             - Only detect marked secret (default=false)
...
```
## Run a test without cache modelling: <br />
```
$ cd litmus/litmus-original/k01/
$ clang-6.0 -g -c -emit-llvm test.c -o test.bc
```   
* "--enable-speculative" option enables the speculative paths exploring.  
* "--max-sew=#" set the Speculative Execution Windows (SEW) to #. In the paper it is called Re-Order Buffer (default is 4, 8, 16, 32 and 64 are used in the paper).

#### For running the examples with **BTB**
```
/PATH/TO/KLEE/ROOT/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=1 -BTB-ways=1 -BTB-tag-bits=0 test.bc
```
You can change the BTB config parameters.

#### For running the examples with **PHT**
```
/PATH/TO/KLEE/ROOT/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=1 test.bc
```

You can change the PHT config parameters.

#### Run a test with cache modelling:
- Add "-enable-cachemodel" option to the command line.
- Cache configurations can be modified by adding the following parameters to command-line instruction.

  ```
  -cache-line-size=#
  -cache-sets=#
  -cache-ways=#
  ```

## Run SpecSym on an example code.
```c
#include <stdint.h>
#include <klee/klee.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 64];
uint8_t temp = 0;


uint8_t victim_fun(int idx)  __attribute__ ((optnone))
{
    int y = 0;
    if (idx < array1_size) {    
        y = array1[idx];
        temp &= array2[y*64];
    }   

    /* This two lines disable the compiler optimization of array */
    array2[0] = 2;  
    array1[0] = 2;
    return temp;
}

int main() {
    int source = 20;
    klee_make_symbolic(&source, sizeof(source), "source");
    victim_fun(source);
    return 0;
}
```
Compile above code to generate bitcode:
```
clang-6.0 -g -c -emit-llvm test.c -o test.bc
```
Run the tool with the generated bitcode: 

#### BTB(1 entry, direct-mapped, no tag bits) - ROB(32 entries)
```
/PATH/TO/KLEE/ROOT/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-btbmodel -BTB-sets=1 -BTB-ways=1 -BTB-tag-bits=0 test.bc
```  

#### BTB(1 entry, direct-mapped, no tag bits) - ROB(16 entries)
```
/PATH/TO/KLEE/ROOT/build/bin/klee -check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-btbmodel -BTB-sets=1 -BTB-ways=1 -BTB-tag-bits=0 test.bc
```

#### PHT(1 bit BHR, 1 entry PHT) - ROB(16 entries)
```
-check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=16 -enable-cachemodel -enable-phtmodel -PHT-reg-size=1 test.bc
```

#### PHT(4 bits BHR, 16 entry PHT) - ROB(32 entries)
```
-check-div-zero=false -check-overshift=false -search=randomsp -enable-speculative -max-sew=32 -enable-cachemodel -enable-phtmodel -PHT-reg-size=4 test.bc
```

The output of SpecSym for PHT(1 bit BHR, 1 entry PHT) and ROB(16 entries):
```
KLEE: WARNING: @Speculative execution modeling is enable! maxSEW=16
KLEE: Using STP solver backend
KLEE: WARNING: 
@Start execution ==>

KLEE: WARNING ONCE: flushing 16384 bytes on read, may be slow and/or crash: MO10[16384] allocated at global:array2
KLEE: @CM: found a leakage: test.c: 22, ASM line=38, time = 32387, Cache ways: 2, Cache set: 256, count=0
KLEE: ----------------------- Leakage Detected --------------------
KLEE: -> test.c: 22, ASMLine: 38, liveness: -1
KLEE: ////////////////////////////////////////////////////////////////////////////////

KLEE: done: total instructions = 65
KLEE: done: completed paths = 2
KLEE: done: sp states without Prediction Logic = 2
KLEE: done: sp states with Prediction Logic = 1
KLEE: done: sp states reduction rate = 50% 
KLEE: done: Common sp states = 1
KLEE: done: Completed sp states = 1
KLEE: done: Average instructions on speculative path = 16
KLEE: done: generated tests = 2
KLEE: done: loads: 14
KLEE: done: stores: 10
KLEE: done: constant loads: 11
KLEE: done: constant stores: 10
```

A description of the output: <br />
The line starts with `KLEE: @CM` denote there is a leakage found by the our tool with branch prediction and cache modeling. The detected leakage can be seen after `Leakage Detected` keyword. The number of lines depends on the pattern. Here, our pattern was a `speculative load intruction` based on secret address. For every node in our pattern, we print the corresponding source code line and llvm-IR line and also we print its liveness or time-to-live if it is defined for the node (`-1` if it is not defined).  
The rest part of the output is the statistic of this test including the numbers of the executed instructions, explored paths, a lower bound on explored speculative paths without prediction logic (`sp states without Prediction Logic = 2`), the exact number of speculative states with prediction logic (`sp states with Prediction Logic = 1`), and the common states between these states (`Common sp states = 1`). 

## Reproduce the data for time measurement.
Then run the bitcode with SpecSym with the command mentioned above with the `--enable-cachemodel` option. You can run `klee-stats` to get the analysis time and the solver time.
```
/PATH/TO/KLEE/ROOT/build/bin/klee-stats klee-out-0
```
Output:
```
-------------------------------------------------------------------------
|   Path   |  Instrs|  Time(s)|  ICov(%)|  BCov(%)|  ICount|  TSolver(%)|
-------------------------------------------------------------------------
|klee-out-0|      65|     0.38|   100.00|   100.00|      43|       82.74|
-------------------------------------------------------------------------
```

# Using SpecSym with Docker
## Building the Docker image locally
```
$ git clone https://github.com/SpecPaper/SpecSym.git
$ cd SpecSym/klee
$ docker build -t SpecSym/SpecSym .
```
## Creating a SpecSym Docker container
```
docker run --rm -ti --ulimit='stack=-1:-1' SpecSym/SpecSym
```
