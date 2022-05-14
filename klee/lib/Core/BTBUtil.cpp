//===-- BTBUtil.cpp  -----------------------------------------------------===//
// 
//
//===----------------------------------------------------------------------===//

#include "BTBUtil.h"

#include <map>
#include <utility>

#define BTB_SET(x,nset) ((x) & (nset - 1))
#define BTB_TAG(x,nset) ((x) >> log_base2(nset))


using namespace llvm;
using namespace klee;

// static class members initilization
/*
AddrPairMap BTBUtil::setPairBuffer;
AddrPairMap BTBUtil::tagPairBuffer;
AddrMap BTBUtil::setBuffer;
AddrMap BTBUtil::tagBuffer;
*/
unsigned int BTBConfig::btb_ways = BTB_WAYS;
unsigned int BTBConfig::btb_set_size = BTB_SET_SIZE;
unsigned int BTBConfig::btb_tag_size = BTB_TAG_SIZE;


// Calculate set for Constant expression
uint64_t BTBUtil::getSet(unsigned addr)  {
    uint64_t set = BTB_SET(addr, BTBConfig::btb_set_size);
    return set;
}


// Calculate tag for Constant expression
uint64_t BTBUtil::getTag(unsigned addr) {
    uint64_t tag =  BTB_TAG(addr, BTBConfig::btb_set_size);
    int i;
    //create a mask for tag bits
    uint64_t temp = 0;
    if(BTBConfig::btb_tag_size != 0){
        for(i = 0; i < BTBConfig::btb_tag_size; i++){
            temp |= 1 << i;
        }
    }

    return (tag & temp);
}

