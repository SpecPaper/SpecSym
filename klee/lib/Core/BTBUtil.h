//===-- BTBUtil.h -------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//
#ifndef __BTB__UTIL__H__
#define __BTB__UTIL__H__

#include "klee/Expr.h"

#include <map>


/* default cache configuration */
#define BTB_WAYS (1)
#define BTB_SET_SIZE (256)
#define BTB_TAG_SIZE (20)

namespace llvm {
    class Constant;
    class ConstantExpr;
}

namespace klee {

/* BTB configuration */
class BTBConfig { 
public:
    static unsigned int btb_ways;
    static unsigned int btb_set_size;
    static unsigned int btb_tag_size;     //number of tags bits that can be stored
};

class BTBUtil {

public:
    static uint64_t getSet(unsigned addr);
    //static ref<Expr> getSet(ref<Expr> &addr);
    static uint64_t getTag(unsigned addr);
    //static ref<Expr> getTag(ref<Expr> &addr);

    static int log_base2(int n) {
        int power = 0;  
        if (n <= 0 || (n & (n-1)) != 0) {
            assert(0 && "log2() only works for positive power of two values");
        }
        while (n >>= 1)
            power++;
        return power;
    }

}; // End class CacheUtil

} // end namespace klee

#endif
