//===-- BTBState.cpp -----------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "klee/PHTState.h"
#include "klee/Internal/Support/ErrorHandling.h"

#include "klee/Internal/Module/InstructionInfoTable.h"
#include "klee/Internal/Module/KInstruction.h"
#include "klee/Internal/Module/KModule.h"
#include "klee/Internal/Module/KInstIterator.h"

#include "llvm/Support/ErrorHandling.h"

#include <iostream>
#include <vector>
#include <memory>

using namespace llvm;
using namespace klee;

/*
    uint32_t* reg;
    std::shared_ptr<uint8_t> table;

    ExecutionState &state;
	uint64_t reg_mask;
*/

unsigned int PHTConfig::pht_reg_size = 8;

PHTState::PHTState(ExecutionState &_state) : state(_state){
	reg = new uint32_t;
	*reg = 0;
	table = std::make_shared<std::vector<uint8_t>>();
	int table_size = 1 << PHTConfig::pht_reg_size;
	for (int i = 0; i < table_size; i++){
		table->push_back(0);
	}
	reg_mask = 0;
	for (int i = 0; i < PHTConfig::pht_reg_size; i++){
		reg_mask = reg_mask << 1;
		reg_mask |= 1;
	}
}

PHTState::PHTState(ExecutionState &_state, const PHTState &cs): state(_state) {
	reg = new uint32_t(*(cs.reg));
	table = std::make_shared<std::vector<uint8_t>>(*(cs.table));
	reg_mask = cs.reg_mask;
}

PHTState::~PHTState(){
	table.reset();
	delete reg;
}

//gets the address and returns the assembly line
bool PHTState::read(){
	switch (table->at(*reg))
	{
	case 0:
		return false;
		break;
	
	case 1:
		return false;
		break;

	case 2:
		return true;
		break;

	case 3:
		return true;
		break;

	default:
		klee_message("UNKNOWN VALUE (read)");
		break;
	}
}

void PHTState::update_table(bool token){
	switch (table->at(*reg))
	{
	case 0:
		table->at(*reg) = token ? 1 : 0;
		break;
	
	case 1:
		table->at(*reg) = token ? 2 : 0;
		break;

	case 2:
		table->at(*reg) = token ? 3 : 1;
		break;

	case 3:
		table->at(*reg) = token ? 3 : 2;
		break;

	default:
		klee_message("UNKNOWN VALUE (table)");
		break;
	}
}

void PHTState::update_reg(bool token){
	uint32_t temp = *reg << 1;
	temp = temp & reg_mask;
	temp |= token ? (uint32_t) 1 : (uint32_t) 0;
	*reg = temp;
}

//storing PC in BTB
void PHTState::update(bool token){
	//update table
	update_table(token);	
	//update reg
	update_reg(token);
}

