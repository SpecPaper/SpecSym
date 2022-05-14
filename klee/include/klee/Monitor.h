#ifndef KLEE_MONITOR_H
#define KLEE_MONITOR_H

#include "klee/Internal/Module/KInstruction.h"
#include "/usr/include/llvm-6.0/llvm/IR/Instruction.h"
#include "/usr/include/llvm-6.0/llvm/IR/Instructions.h"
#include "/usr/include/llvm-6.0/llvm/IR/IntrinsicInst.h"

#include "klee/ExecutionState.h"
#include "../../lib/Core/TimingSolver.h"

#include<vector>
#include<list>
#include<set>
#include<string>

namespace llvm {
  class Instruction;
}

namespace klee{
struct InstructionInfo;

//0: don't have, 1:must have, o.w.: don't care

struct updateInfo {
    ExecutionState &state;
    TimingSolver *solver;
    KInstruction *ki;
    //
    std::string instruction;
    uint8_t isSpec;
    uint8_t isSensitive;
    uint8_t isConst;
    //uint8_t checkCacheState;

    updateInfo(ExecutionState &_state, TimingSolver *_solver, KInstruction *_ki):
        state(_state), solver(_solver), ki(_ki), isSpec(2), isSensitive(2), isConst(2)
        {
            switch (_ki->inst->getOpcode())
            {
            case llvm::Instruction::Br:
                instruction = "Br";
                break;
            case llvm::Instruction::Load:
                instruction = "Load";
                break;
            case llvm::Instruction::Store:
                instruction = "Store";
                break;
            case llvm::Instruction::Call:
                instruction = "Call";
                break;
            default:
                instruction = "Other";
                break;
            }
        }
};

struct fsmNode{
    ///0,1,2,3,4,...
    uint8_t label;

    //speculative. 0: Real, 1: Speculative, o.w.: Don't care.
    uint8_t isSpeculative;
    //TTL
    int startTTL;  //should we start ttl from this node?
    bool stopTTL;   //should we stop/reset ttl at this node?
    ///instructions, the result should be in form of Instruction::Add.
    ///Possible instructions to follow is:
    ///     Branch (direct, indirect, switch-case)
    ///     Load, Store operations.
    std::string instruction;

    //instruction operand

    //sensitive
    uint8_t isSensitive;
    
    //checks whether something (based on the instruction) is const or not
    uint8_t isConst;
    //cacheState. check the cache state for an address or a secret.
    uint8_t checkCacheState;
    
    fsmNode(uint8_t _label, uint8_t _isSpeculative, int _startTTL, bool _stopTTL, std::string _instruction, uint8_t _isSensitive, uint8_t _checkCacheState, uint8_t _isConst):
        label(_label), isSpeculative(_isSpeculative), startTTL(_startTTL), stopTTL(_stopTTL), instruction(_instruction), isSensitive(_isSensitive), checkCacheState(_checkCacheState), isConst(_isConst) {}
    
    fsmNode(uint8_t _label): label(_label) {}
};

//This is a structure to create, track, and store FSM.
//The information needed to pass from executor is: isSpeculative, KInstruction, 

class FSM{

public:
    //This is used to create an FSM to capture. Linked list.
    std::vector<fsmNode*> nodes;
    int num_nodes;

    /// To create an FSM from a pattern file, InputPattern.c_str should be passed to the runFunctionAsMain to create it.
    /// Then it can be processed in ExecutionState constructor.
    FSM();
    ~FSM();

    void addNode();
    void update();
    //gets the current node label, and gives us the label of the next node. 
    std::string nextNodeLabel();
};

///initial Node of a trace.
struct iTrace{
    const InstructionInfo* info; 
    int ttl;
    //number of available parents
    int pNum;

    //constructors
    iTrace(const InstructionInfo* i, int t, int p): info(i), ttl(t), pNum(p) {} 
    
    //destructor
    ~iTrace() {}
};

class Monitor{
//private:
    FSM* fsm;

    ExecutionState* state;
    ExecutionState* returnState;

    std::vector<std::list<std::shared_ptr<iTrace>>> patterns;

    std::set<std::vector<std::shared_ptr<iTrace>>> detectedPatterns;

    void reduceParent(int index); //remove parents of patterns[index]
    void removePatterns(int index); // remove patterns [index, end)
    void moveToDetected(const InstructionInfo* i);
    uint8_t firstEmpty();
    bool checkEqual(fsmNode* a, struct updateInfo &uInfo);


public:
    Monitor(ExecutionState* es);
    Monitor(ExecutionState* es, const Monitor &mn);
    ~Monitor();
    
    void setReturnState(ExecutionState *st);

    void update(struct updateInfo &uInfo);
    void step();
    void copyMonitor();
    //print the number of detected patterns for each node
    void printNumPat();
};

}

#endif