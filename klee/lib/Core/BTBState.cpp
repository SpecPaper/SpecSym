//===-- BTBState.cpp -----------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "klee/BTBState.h"
#include "BTBUtil.h"
#include "klee/Internal/Support/ErrorHandling.h"

#include "klee/Internal/Module/InstructionInfoTable.h"
#include "klee/Internal/Module/KInstruction.h"
#include "klee/Internal/Module/KModule.h"
#include "klee/Internal/Module/KInstIterator.h"

#include "llvm/Support/ErrorHandling.h"

#include <iostream> 
#include <time.h>

using namespace llvm;
using namespace klee;

//constructor
BTBSet::BTBSet(uint8_t ways){
	//llvm::errs() << "Create cnt: ";
	for (uint8_t i = 0; i < ways; i++){
		cnt.push_back(ways-1);
		tagBTB.push_back(0);
		BTB.push_back(nullptr);
	}
}

//deep copy constructor
BTBSet::BTBSet(uint8_t ways, const BTBSet &s){
	for (uint8_t i = 0; i < ways; i++){
		cnt.push_back(s.cnt[i]);
		tagBTB.push_back(s.tagBTB[i]);
		BTB.push_back(s.BTB[i]);
	}
	// llvm::errs() << "Pass cnt: ";
	// for (int i = 0; i < ways; i++)
	// {
	// 	llvm::errs() << (unsigned)cnt[i] << "\t";
	// }
	// llvm::errs() << "\n";
}

//destructor
BTBSet::~BTBSet() {}

void BTBSet::upCnt(int threshold, uint8_t way){
	for (uint8_t i = 0; i < way; i++)
	{
		if(cnt[i] < threshold) cnt[i]++;
	}
}

void BTBSet::upCnt(uint8_t way){
	for (uint8_t i = 0; i < way; i++)
	{
		if(cnt[i] < way-1) cnt[i]++;
	}
}

bool BTBSet::isFull(uint8_t way){
	for (uint8_t i = 0; i < way; i++)
	{
		if(BTB[i] == nullptr){
			return false;
		}
	}
	return true;
}

void BTBSet::dump(uint8_t way){
	llvm::errs() << "Dump cnt( " << (unsigned)way << "): ";
	for (uint8_t i = 0; i < way; i++)
	{
		llvm::errs() << (unsigned)cnt[i] << "\t";
	}
	llvm::errs() << "\n";
}

KInstIterator BTBSet::getPC(uint64_t tag, uint8_t way){
	//llvm::errs() << "read cnt( " << (unsigned)way << ")\n";
	for (uint8_t i = 0; i < way; i++)
	{
		if((tagBTB[i] == tag) && (BTB[i] != nullptr)){
			upCnt(cnt[i], way);
			cnt[i] = 0;
			//llvm::errs() << "Read\n";
			// for (uint8_t i = 0; i < way; i++)
			// {
			// 	llvm::errs() << (unsigned)cnt[i] << "\t";
			// }
			// llvm::errs() << "\n";
			return BTB[i];
		}
	}
	return nullptr;
}

void BTBSet::update(uint64_t tag, const KInstIterator &pc, uint8_t way){
	//llvm::errs() << "update cnt( " << (unsigned)way << ")\n";
	bool full = isFull(way);
	//check if it exists in set
	for (uint8_t i = 0; i < way; i++)
	{
		if(tagBTB[i] == tag){
			if(BTB[i] != nullptr){
				upCnt(cnt[i], way);
				cnt[i] = 0;
				BTB[i] = pc;
				//llvm::errs() << "Update (exist):" << (unsigned)i << "\n";
				// for (uint8_t i = 0; i < way; i++)
				// {
				// 	llvm::errs() << (unsigned)cnt[i] << "\t";
				// }
				// llvm::errs() << "\n";
				return;
			}
		}
	}
	//it is not in the set
	for (uint8_t i = 0; i < way; i++)
	{
		if(cnt[i] == way-1){
			upCnt(way);
			cnt[i] = 0;
			BTB[i] = pc;
			/*
			llvm::errs() << "Update (no exist):" << (unsigned)i << "\n";
			for (uint8_t i = 0; i < way; i++)
			{
				llvm::errs() << (unsigned)cnt[i] << "\t";
			}
			llvm::errs() << "\n";
			*/
			return;
		}
	}
	//dump(way);
	//klee_message("CNT=way-1 is not found");
	assert(false);
}

///////////////////////////////////BTB State///////////////////////////////////////
BTBState::BTBState(ExecutionState &_state) : state(_state){
	set_size = BTBConfig::btb_set_size;
	set_bits = BTBUtil::log_base2(set_size);
	tag_bits = BTBConfig::btb_tag_size;
	ways = BTBConfig::btb_ways;

	for (int i = 0; i < set_size; i++){
		btb.push_back(BTBSet(ways));
	}
}

BTBState::BTBState(ExecutionState &_state, const BTBState &cs): state(_state) {
	set_size = BTBConfig::btb_set_size;
	set_bits = BTBUtil::log_base2(set_size);
	tag_bits = BTBConfig::btb_tag_size;
	ways = BTBConfig::btb_ways;

	for (int i = 0; i < set_size; i++){
		btb.push_back(BTBSet(ways, cs.btb[i]));
	}
}

//gets the address and returns the assembly line
KInstIterator BTBState::read(uint64_t addr){
	uint64_t set_num = BTBUtil::getSet(addr);
	uint64_t tag = 0;
	if(tag_bits > 0){
		tag = BTBUtil::getTag(addr);
	}
	/*
	llvm::errs() << "Read:\n";
	for (int i = 0; i < set_size; i++)
	{
		for (int j = 0; j < ways; j++)
		{
			llvm::errs() << (unsigned)btb[i].cnt[j] << "\t";
		}
		llvm::errs() << "\n";
	}
	*/
	return btb[set_num].getPC(tag, ways);
}

//gets address, dst (e.g., bi->getSuccessor(0)), and program state.
//Computes set and tag bits.
//Based on dst and state, computes assembly line of the desired next instruction.

//storing PC in BTB
void BTBState::update(uint64_t addr, KInstIterator &btbPC, ExecutionState &state){
    //compute tag and set
	uint64_t set = BTBUtil::getSet(addr);
	uint64_t tag = BTBUtil::getTag(addr);

    //compute pc
    // KFunction *kf = state.stack.back().kf;
    // unsigned entry = kf->basicBlockEntry[dst];
    // KInstIterator pc = &kf->instructions[entry];

	KInstIterator pc = btbPC;

	btb[set].update(tag, pc, ways);
	/*
	llvm::errs() << "Update Set: " << set << "\n";
	for (int i = 0; i < set_size; i++)
	{
		for (int j = 0; j < ways; j++)
		{
			llvm::errs() << (unsigned)btb[i].cnt[j] << "\t";
		}
		llvm::errs() << "\n";
	}
	*/
	return;
}
