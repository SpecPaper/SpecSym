//===-- BTBState.h ------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//
#ifndef KLEE_PHTSTATE_H
#define KLEE_PHTSTATE_H

#include "klee/ExecutionState.h"

#include <memory>
#include <vector> 
#include <set>
#include <map>
#include <unordered_map> 

namespace llvm {
  class BasicBlock;
}

namespace klee {
        class Expr;
        template<class T> class ref;

class PHTConfig { 
public:
    static unsigned int pht_reg_size;
};


class PHTState {

public:
    uint32_t* reg;
    std::shared_ptr<std::vector<uint8_t>> table;

private:
    ExecutionState &state;
	uint64_t reg_mask;

    void update_table(bool token);
    void update_reg(bool token);
public:
	//cacheState(uint64_t set_size, uint64_t set_bits, uint64_t tag_bits);
    // set size is in utils.h -- set bits with log2, tag_bits is more difficult to calculate
    PHTState(ExecutionState &state);
    PHTState(ExecutionState &state, const PHTState &cs);
    ~PHTState();

	//bool isConflict(ConstantExpr *addr1, ConstantExpr *addr2);
	
    //static uint64_t getSet(ConstantExpr *ce);
    //static uint64_t getTag(ConstantExpr *ce);
    
	//uint64_t compute_tag(uint64_t addr);
	//uint64_t compute_set(uint64_t addr);
	
	bool read(); 
	void update(bool taken);

	//for testing
	//void print_BTB();
};

} // end namespace klee

#endif
