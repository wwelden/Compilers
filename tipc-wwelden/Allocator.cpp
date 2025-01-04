#include <vector>
#include <limits>
#include <stack>
#include <iomanip>
#include <stdlib.h>
#include <optional>
#include <iostream>

#include "compilerUtils.h"
#include "scanner.h"
#include "parser.h"
#include "Allocator.h"

const int INVALID = -1;
const int Infinity = -100; //std::numeric_limits<int>::max();
int memAddr = 32768;
vector<Operand> instructionList;

Allocator::Allocator(int k) { // might want to change some of this
    VRName = 0;
    if(debugLevel > 1) {
      cout << "//" << k << endl;
    }
    numPRs = k;
    for (int i = numPRs-1; i > 0; i--) {
        FreePRs.push(i);
    }
}

int getInfinity() {
    return Infinity;
}


// this is used in the Compute Last Use algorithm
    void Allocator::Update(Operand &op, int index) {
        if(op.isReg) {
            if (SRtoVR[op.SR] == INVALID) {
                SRtoVR[op.SR] = VRName++;
            }
            op.VR = SRtoVR[op.SR];  // Assuming there's a VR field in Operand
            op.NU = LastUse[op.SR]; // Assuming there's a NU field in Operand
            LastUse[op.SR] = index;
        }
    }

// need more update functions - not just for SRtoVR (?)

//function to find the biggest vr/ sr in the block

    int Allocator::biggestReg(std::vector<Line> &Block) {
        int maxRegNum = 0;
        for (Line &inst : Block) {
            if (inst.op1.isReg) {
                if (inst.op1.VR > maxRegNum) {
                    maxRegNum = inst.op1.VR;
                }
            }
            if (inst.op2.isReg) {
                if (inst.op2.VR > maxRegNum) {
                    maxRegNum = inst.op2.VR;
                }
            }
            if (inst.dest.isReg) {
                if (inst.dest.VR > maxRegNum) {
                    maxRegNum = inst.dest.VR;
                }
            }
            if (inst.op1.isReg) {
                if (inst.op1.SR > maxRegNum) {
                    maxRegNum = inst.op1.SR;
                }
            }
            if (inst.op2.isReg) {
                if (inst.op2.SR > maxRegNum) {
                    maxRegNum = inst.op2.SR;
                }
            }
            if (inst.dest.isReg) {
                if (inst.dest.SR > maxRegNum) {
                    maxRegNum = inst.dest.SR;
                }
            }
        }
        return maxRegNum;
    }

    void Allocator::ComputeLastUse(std::vector<Line> &Block) {
        int n = Block.size();
        int maxRegNum = biggestReg(Block);
        SRtoVR.resize(maxRegNum, INVALID);
        LastUse.resize(maxRegNum, Infinity);
        //biggestReg(Block);

        // n =/= "max register number"?
        // n is num of lines not registers
        // max reg might equal num regs
        for (int i = 0; i < n; i++) { // Assuming there are n+1 registers
            SRtoVR[i] = INVALID;
            LastUse[i] = Infinity;
        }

        // dest = OP3? Ya
        for (int i = n-1; i >= 0; i--) {
            Update(Block[i].dest, i);
            if (Block[i].dest.isReg) {
                SRtoVR[Block[i].dest.SR] = INVALID;
                LastUse[Block[i].dest.SR] = Infinity;
            }
            Update(Block[i].op1, i); // The algorithm updates twice, assuming it's for op1 and op2
            Update(Block[i].op2, i);
        }
    }


    // This is a stub; actual restoration logic would depend on architecture and context.
  
    // isReg, SR, constant, VR, PR, NU
    void Allocator::restore(int vr) {
        // Logic to insert restore instructions for pr before inst.
        if(debugLevel > 1){
        cout << "//restoring VR: " << vr << " to PR " << VRtoPR[vr] << endl;
        }
        int newMemAddr = VRtoMemAd[vr];
        allocatedBlock.push_back(Line{LOADI, makeOpperand(false, -1, newMemAddr), makeOpperand(false, -1, -1), makeOpperand(true, -1, -1, -1, 0), -1});// loadI memadd into r2?
        allocatedBlock.push_back(Line{LOAD, makeOpperand(true, -1, -1, -1, 0), makeOpperand(false, -1, -1), makeOpperand(true, -1, -1, -1, VRtoPR[vr]), -1});// load r2 into pr0
    }

    // This is a stub; actual spilling logic would depend on architecture and context.
    // int Allocator::spill(const Operand& inst) {
    int Allocator::spill() {
        // Logic to find the PR with the furthest NU and spill it.
        // Return the chosen PR.

        //find the pr with the furthest NU

        // loop through numRegs and find the PR with the furthest NU
        int furthestNU = -2; // -2 because because we use -1 for invalid
        int furthestPR = -2;
        for(int i = 1; i < numPRs; i++) {
            if(PRtoNU[i] > furthestNU) {
                furthestNU = PRtoNU[i];
                furthestPR = i;
            }
        }
        //spill it
        if(debugLevel > 1){
        cout << "//spilling PR: " << furthestPR << endl;
        }
        allocatedBlock.push_back(Line{LOADI, makeOpperand(false, -1, memAddr), makeOpperand(false, -1, -1), makeOpperand(true, -1, -1, -1, 0), -1}); //memadd goes here 
        allocatedBlock.push_back(Line{STORE, makeOpperand(true, -1, -1,PRtoVR[furthestPR],furthestPR), makeOpperand(true, -1, -1, -1, 0), makeOpperand(false, -1, -1), -1});
        VRtoPR[PRtoVR[furthestPR]] = INVALID;
        VRtoMemAd[PRtoVR[furthestPR]] = memAddr;
        memAddr += 4;
        return furthestPR;
    }

    // int Allocator::GetPR(const Operand& inst) {
    int Allocator::GetPR() {
        int pr;
        if (!FreePRs.empty()) {
            pr = FreePRs.top();
            FreePRs.pop();
        } else {
            pr = spill();
        }
        assertCondition(0 < pr < numPRs, "Invalid PR found in getPR.");
        return pr;
    }
    void Allocator::ensurePR(Operand &op)
    {
        if (op.isReg)
        {
            if (VRtoPR[op.VR] == INVALID)
            {
                VRtoPR[op.VR] = GetPR();
                PRtoVR[VRtoPR[op.VR]] = op.VR;
                restore(op.VR);
                PRtoNU[VRtoPR[op.VR]] = INVALID;
            }
            op.PR = VRtoPR[op.VR];
        }
    }
    void Allocator::handleNU(Operand &op)
    {
        if (op.isReg)
        {
            if (op.NU == Infinity)
            {
                FreePRs.push(VRtoPR[op.VR]);
                VRtoPR[op.VR] = INVALID;
                PRtoVR[op.PR] = INVALID;
                PRtoNU[op.PR] = Infinity;
            }
            else
            {
                PRtoNU[op.PR] = op.NU;
            }
        }
    }

// when you free a pr erase its NU
// Initiallize NU to infinity
// find the operand when you need to update it
// only spill when free prs is empty
    void Allocator::printTables() {
      if(debugLevel > 1) {
        printSummary(VRtoPR, INVALID, "VRtoPR");
        printSummary(VRtoMemAd, INVALID, "VRtoMemAd");
        printSummary(PRtoVR, INVALID, "PRtoVR");
        printSummary(PRtoNU, Infinity, "PRtoNU");
        cout << "//FreePRS: ";
        printStack(FreePRs);
        cout << endl;
      }
    }


    void Allocator::LocalRegisterAllocation(std::vector<Line>& Block) {
        // dr. fogarty said this line was fine
        //int maxVR = findMaxVR(Block);
        int maxRegNum = biggestReg(Block);
        VRtoPR.resize(maxRegNum, INVALID);
        //numPRS
        PRtoVR.resize(maxRegNum, INVALID);
        PRtoNU.resize(maxRegNum, Infinity);
        VRtoMemAd.resize(maxRegNum, INVALID);
        //biggestReg(Block);

        // Assuming FreePRs gets populated with non-reserved registers somewhere.
        for (Line &inst : Block)
        {
          printTables();
          //inst.tablePrint();
            // no guarantee there is two ops (ex. loadI, I think)
            // if statement (or maybe cases) to check how many ops there are?
            
          // make this a funciton ensurePR(Operand &op)

            ensurePR(inst.op1);
            ensurePR(inst.op2);
            //make this a function handleNU(Operand &op)
            handleNU(inst.op1);
            handleNU(inst.op2);

            // this commented line was with itself but flipped - should be fixed (I think) as of 11/28
            // VRtoPR[inst.dest.VR] = inst.dest.PR;
            if(inst.dest.isReg) {
                VRtoPR[inst.dest.VR] = GetPR();
                inst.dest.PR = VRtoPR[inst.dest.VR];
                PRtoVR[inst.dest.PR] = inst.dest.VR;
                // Update(inst.dest, inst.lineNum);
                PRtoNU[inst.dest.PR] = inst.dest.NU; 
           }
          if(debugLevel > 1)
              inst.tablePrint();
          allocatedBlock.push_back(inst);
        }
    }



//idk why there is a main function here probalby should have something in the main file instead
    // int Allocator::main() {
    //     std::vector<Line> Block; // Assuming this gets populated with Operand instructions somewhere.
    //     ComputeLastUse(Block);
    //     LocalRegisterAllocation(Block);

    //     return 0;
    // }

