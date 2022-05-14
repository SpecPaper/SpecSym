//===-- BTBState.h ------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//
#ifndef KLEE_BTBSTATE_H
#define KLEE_BTBSTATE_H


#include "ExecutionState.h"
#include "../../lib/Core/BTBUtil.h"
#include "llvm/IR/BasicBlock.h"

#include "klee/Internal/Module/InstructionInfoTable.h"
#include "klee/Internal/Module/KInstruction.h"
#include "klee/Internal/Module/KModule.h"
#include "klee/Internal/Module/KInstIterator.h"

#include <vector> 
#include <set>
#include <map>
#include <unordered_map> 

namespace llvm {
  class BasicBlock;
}

namespace klee {
    class InstructionInfoTable;
    struct KFunction;
    struct KInstruction;
    class KInstIterator;
    class KModule;

struct BTBSet {
    //counter for unsigned.
    std::vector<uint8_t> cnt;
    // tag bits
    std::vector<uint64_t> tagBTB;
    //PC 
    std::vector<KInstIterator> BTB;
    //dummy constructor
    BTBSet() {}
    //constructor
    BTBSet(uint8_t ways);
    //deep copy constructor
    BTBSet(uint8_t ways, const BTBSet &s);
    //destructor
    ~BTBSet();
    // get tag and return KInstIterator
    KInstIterator getPC(uint64_t tag, uint8_t way);
    //update PC
    void update(uint64_t tag, const KInstIterator &pc, uint8_t way);

private:
    //update cnt if it's the last element
    void upCnt(uint8_t way);
    //update cnt if it's not the last element
    void upCnt(int index, uint8_t way);
    //returns true if all ways are full
    bool isFull(uint8_t way);
    //dump array
    void dump(uint8_t way);
};

class BTBState {

public:
    //Storing PC in BTB
    // KInstIterator *BTB;
    // uint64_t *tagBTB;
    std::vector<BTBSet> btb;
    //BTBSet* btb;

private:
	// pair of tag and labels
	//std::vector<std::pair <long int, std::string>> cState;
	// mapping of address and label.
    ExecutionState &state;
    uint64_t set_size;
	uint64_t set_bits;
	uint64_t tag_bits;
    uint8_t ways;

public:
	//cacheState(uint64_t set_size, uint64_t set_bits, uint64_t tag_bits);
    // set size is in utils.h -- set bits with log2, tag_bits is more difficult to calculate
    BTBState(ExecutionState &state);
    BTBState(ExecutionState &state, const BTBState &cs);
	
	KInstIterator read(uint64_t addr);
	void update(uint64_t addr, KInstIterator &btbPC, ExecutionState &state);

	//for testing
	//void print_BTB();
};

} // end namespace klee

#endif
