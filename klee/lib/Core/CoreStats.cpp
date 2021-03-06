//===-- CoreStats.cpp -----------------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "CoreStats.h"

using namespace klee;

Statistic stats::allocations("Allocations", "Alloc");
Statistic stats::coveredInstructions("CoveredInstructions", "Icov");
Statistic stats::falseBranches("FalseBranches", "Bf");
Statistic stats::forkTime("ForkTime", "Ftime");
Statistic stats::forks("Forks", "Forks");
Statistic stats::instructionRealTime("InstructionRealTimes", "Ireal");
Statistic stats::instructionTime("InstructionTimes", "Itime");
Statistic stats::instructions("Instructions", "I");
Statistic stats::minDistToReturn("MinDistToReturn", "Rdist");
Statistic stats::minDistToUncovered("MinDistToUncovered", "UCdist");
Statistic stats::reachableUncovered("ReachableUncovered", "IuncovReach");
Statistic stats::resolveTime("ResolveTime", "Rtime");
Statistic stats::solverTime("SolverTime", "Stime");
Statistic stats::states("States", "States");
Statistic stats::trueBranches("TrueBranches", "Bt");
Statistic stats::uncoveredInstructions("UncoveredInstructions", "Iuncov");

/* SPSE */

Statistic stats::loads("Loads", "Loads");
Statistic stats::stores("Stores", "Stores");

Statistic stats::constLoads("Cloads", "Cloads");
Statistic stats::constStores("Cstores", "Cstores");

Statistic stats::spLoads("SPload", "SPload");
Statistic stats::spStores("SPstores", "SPstores");

Statistic stats::spStates("Spstates", "Spstates");
//spec states with prediction logic
Statistic stats::spStatesPL("SpstatesPL", "SpstatesPL");
//common spec state between thisTool and kleespectre
Statistic stats::spStatesCommon("SpstatesCommon", "SpstatesCommon");
Statistic stats::spStatesDone("CompleteSpstates", "CompleteSpstates");
Statistic stats::totalSpecInst("TotalSpecInst", "TotalSpecInst");


