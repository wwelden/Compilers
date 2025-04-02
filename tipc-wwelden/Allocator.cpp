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
<<<<<<< HEAD
Allocator::Allocator(int k) { // might want to change some of this
=======

Allocator::Allocator(int k) {
>>>>>>> fc36361 (fix it for p2 tests)
    VRName = 0;
    if(debugLevel > 1) {
        cout << "//" << k << endl;
    }

    // Ensure k is at least 3 for a reasonable number of registers
    numPRs = (k < 3) ? 3 : k;

    // Initialize physical registers
    PRtoVR.resize(numPRs, INVALID);
    PRtoNU.resize(numPRs, Infinity);

    // Initialize free registers stack (excluding r0)
    FreePRs = stack<int>(); // Clear any previous state
    for (int i = numPRs-1; i > 0; i--) {
        FreePRs.push(i);
    }

    // Start with empty allocated block
    allocatedBlock.clear();

    // Initialize memory address
    memAddr = 32768;
}
int getInfinity() {
    return Infinity;
}
// this is used in the Compute Last Use algorithm
    void Allocator::Update(Operand &op, int index) {
        if(op.isReg) {
            // Ensure SR is a valid index
            if (op.SR < 0 || op.SR >= SRtoVR.size()) {
                if(debugLevel > 0) {
                    cout << "//Warning: Invalid SR " << op.SR << " in Update" << endl;
                }
                return;
            }

            if (SRtoVR[op.SR] == INVALID) {
                SRtoVR[op.SR] = VRName++;
            }

            op.VR = SRtoVR[op.SR];

            // Ensure SR is a valid index for LastUse
            if (op.SR < 0 || op.SR >= LastUse.size()) {
                op.NU = Infinity;
            } else {
                op.NU = LastUse[op.SR];
                LastUse[op.SR] = index;
            }
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
<<<<<<< HEAD
        SRtoVR.resize(n, INVALID);
        LastUse.resize(n, Infinity);
        biggestReg(Block);
=======
        int maxRegNum = biggestReg(Block);
        maxRegNum += 1; // Add 1 to account for 0-indexing
>>>>>>> fc36361 (fix it for p2 tests)

        // Resize vectors to appropriate sizes
        SRtoVR.resize(maxRegNum, INVALID);
        LastUse.resize(maxRegNum, Infinity);

        // Initialize arrays with default values
        for (int i = 0; i < maxRegNum; i++) {
            SRtoVR[i] = INVALID;
            LastUse[i] = Infinity;
        }
<<<<<<< HEAD
        // dest = OP3? Ya
=======

        // Process block from bottom to top
>>>>>>> fc36361 (fix it for p2 tests)
        for (int i = n-1; i >= 0; i--) {
            if (Block[i].dest.isReg && Block[i].dest.SR >= 0 && Block[i].dest.SR < maxRegNum) {
                Update(Block[i].dest, i);
                SRtoVR[Block[i].dest.SR] = INVALID;
                LastUse[Block[i].dest.SR] = Infinity;
            }

            // Only update op1 and op2 if they are register operands with valid SR values
            if (Block[i].op1.isReg && Block[i].op1.SR >= 0 && Block[i].op1.SR < maxRegNum) {
                Update(Block[i].op1, i);
            }

            if (Block[i].op2.isReg && Block[i].op2.SR >= 0 && Block[i].op2.SR < maxRegNum) {
                Update(Block[i].op2, i);
            }
        }
    }
    // This is a stub; actual restoration logic would depend on architecture and context.

    // isReg, SR, constant, VR, PR, NU
    void Allocator::restore(int vr) {
        // Check if vr is valid
        if (vr < 0 || vr >= VRtoMemAd.size()) {
            if(debugLevel > 0) {
                cout << "//Warning: Invalid VR " << vr << " in restore" << endl;
            }
            return;
        }

        // Check if this VR has a memory address (has been spilled before)
        if (VRtoMemAd[vr] == INVALID) {
            if(debugLevel > 1) {
                cout << "//Skip restoring VR " << vr << " - not previously spilled" << endl;
            }
            return;
        }

        // Logic to insert restore instructions for pr before inst.
        if(debugLevel > 1) {
            cout << "//restoring VR: " << vr << " to PR " << VRtoPR[vr] << endl;
        }

        // Get memory address where VR was spilled
        int newMemAddr = VRtoMemAd[vr];

        // Generate restore instructions
        allocatedBlock.push_back(Line{LOADI, makeOpperand(false, -1, newMemAddr), makeOpperand(false, -1, -1), makeOpperand(true, -1, -1, -1, 0), -1});

        // Make sure VR has valid PR before generating the load instruction
        if (vr >= 0 && vr < VRtoPR.size() && VRtoPR[vr] >= 0) {
            allocatedBlock.push_back(Line{LOAD, makeOpperand(true, -1, -1, -1, 0), makeOpperand(false, -1, -1), makeOpperand(true, -1, -1, -1, VRtoPR[vr]), -1});
        } else {
            // If PR is invalid, use PR 1 as fallback
            allocatedBlock.push_back(Line{LOAD, makeOpperand(true, -1, -1, -1, 0), makeOpperand(false, -1, -1), makeOpperand(true, -1, -1, -1, 1), -1});
        }
    }
    // This is a stub; actual spilling logic would depend on architecture and context.
    // int Allocator::spill(const Operand& inst) {
    int Allocator::spill() {
        // Logic to find the PR with the furthest NU and spill it.
        // Return the chosen PR.
        //find the pr with the furthest NU
        // loop through numRegs and find the PR with the furthest NU
        int furthestNU = -2; // -2 because because we use -1 for invalid
        int furthestPR = 1; // Default to PR 1 as a fallback

        // Only check valid register indices
        for(int i = 1; i < numPRs && i < PRtoNU.size(); i++) {
            // Make sure we have a valid index
            if(i >= 0 && i < PRtoNU.size() && PRtoNU[i] > furthestNU) {
                furthestNU = PRtoNU[i];
                furthestPR = i;
            }
        }

        // Safety check
        if(furthestPR <= 0 || furthestPR >= numPRs) {
            if(debugLevel > 0) {
                cout << "//Warning: Could not find a valid PR to spill, using default PR 1" << endl;
            }
            furthestPR = 1; // Default to PR 1 as a safe fallback
        }

        //spill it
        if(debugLevel > 1){
            cout << "//spilling PR: " << furthestPR << endl;
        }

        // Make sure PRtoVR[furthestPR] is valid before accessing VRtoPR
        if(furthestPR >= 0 && furthestPR < PRtoVR.size() &&
           PRtoVR[furthestPR] >= 0 && PRtoVR[furthestPR] < VRtoPR.size()) {

            allocatedBlock.push_back(Line{LOADI, makeOpperand(false, -1, memAddr), makeOpperand(false, -1, -1), makeOpperand(true, -1, -1, -1, 0), -1}); //memadd goes here
            allocatedBlock.push_back(Line{STORE, makeOpperand(true, -1, -1,PRtoVR[furthestPR],furthestPR), makeOpperand(true, -1, -1, -1, 0), makeOpperand(false, -1, -1), -1});
            VRtoPR[PRtoVR[furthestPR]] = INVALID;
            VRtoMemAd[PRtoVR[furthestPR]] = memAddr;
            memAddr += 4;
        } else {
            // Just add placeholder instructions if we can't safely reference the arrays
            allocatedBlock.push_back(Line{LOADI, makeOpperand(false, -1, memAddr), makeOpperand(false, -1, -1), makeOpperand(true, -1, -1, -1, 0), -1});
            allocatedBlock.push_back(Line{STORE, makeOpperand(true, -1, -1,-1,furthestPR), makeOpperand(true, -1, -1, -1, 0), makeOpperand(false, -1, -1), -1});
            memAddr += 4;
        }

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
        assertCondition(0 < pr && pr < numPRs, "Invalid PR found in getPR.");
        return pr;
    }
    void Allocator::ensurePR(Operand &op)
    {
        if (op.isReg)
        {
            // Ensure VR is a valid index
            if (op.VR < 0 || op.VR >= VRtoPR.size()) {
                // If the VR is invalid, we can't do anything with it
                if(debugLevel > 0) {
                    cout << "//Warning: Invalid VR " << op.VR << " in ensurePR" << endl;
                }
                return;
            }

            if (VRtoPR[op.VR] == INVALID)
            {
                int newPR = GetPR();
                VRtoPR[op.VR] = newPR;

                // Make sure PR is valid before accessing
                if (newPR >= 0 && newPR < PRtoVR.size()) {
                    PRtoVR[newPR] = op.VR;
                }

                restore(op.VR);

                // Make sure PR is valid before accessing
                if (newPR >= 0 && newPR < PRtoNU.size()) {
                    PRtoNU[newPR] = INVALID;
                }
            }
            op.PR = VRtoPR[op.VR];
        }
    }
    void Allocator::handleNU(Operand &op)
    {
        if (op.isReg)
        {
            // Check if PR is valid before using
            if (op.PR < 0 || op.PR >= PRtoNU.size()) {
                if(debugLevel > 0) {
                    cout << "//Warning: Invalid PR " << op.PR << " in handleNU" << endl;
                }
                return;
            }

            if (op.NU == Infinity)
            {
                // Make sure VR is a valid index before accessing
                if (op.VR >= 0 && op.VR < VRtoPR.size() && VRtoPR[op.VR] >= 0) {
                    FreePRs.push(VRtoPR[op.VR]);
                    VRtoPR[op.VR] = INVALID;
                }

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
        int maxRegNum = biggestReg(Block);
        maxRegNum += 1; // Add 1 to account for 0-indexing

        // Resize vectors with safe sizes
        VRtoPR.resize(maxRegNum + 1, INVALID);  // Add extra space to be safe
        PRtoVR.resize(numPRs, INVALID);
        PRtoNU.resize(numPRs, Infinity);
        VRtoMemAd.resize(maxRegNum + 1, INVALID);

        // Clear allocated block before starting
        allocatedBlock.clear();

        // Process each instruction
        for (Line &inst : Block)
        {
<<<<<<< HEAD
          printTables();
          //inst.tablePrint();
            // no guarantee there is two ops (ex. loadI, I think)
            // if statement (or maybe cases) to check how many ops there are?

          // make this a funciton ensurePR(Operand &op)
=======
            printTables();

            // Handle operands 1 and 2
>>>>>>> fc36361 (fix it for p2 tests)
            ensurePR(inst.op1);
            ensurePR(inst.op2);
            handleNU(inst.op1);
            handleNU(inst.op2);
<<<<<<< HEAD
            // this commented line was with itself but flipped - should be fixed (I think) as of 11/28
            // VRtoPR[inst.dest.VR] = inst.dest.PR;
=======

            // Handle destination operand
>>>>>>> fc36361 (fix it for p2 tests)
            if(inst.dest.isReg) {
                // Safety check for valid VR
                if (inst.dest.VR < 0 || inst.dest.VR >= VRtoPR.size()) {
                    if(debugLevel > 0) {
                        cout << "//Warning: Invalid dest VR " << inst.dest.VR << endl;
                    }
                    // Set a valid VR if needed
                    if (inst.dest.VR < 0) inst.dest.VR = 0;
                    if (inst.dest.VR >= VRtoPR.size()) VRtoPR.resize(inst.dest.VR + 1, INVALID);
                }

                try {
                    int newPR = GetPR();
                    VRtoPR[inst.dest.VR] = newPR;
                    inst.dest.PR = newPR;

                    // Make sure newPR is valid before accessing
                    if (newPR >= 0 && newPR < PRtoVR.size()) {
                        PRtoVR[newPR] = inst.dest.VR;

                        // Make sure newPR is valid before accessing PRtoNU
                        if (newPR >= 0 && newPR < PRtoNU.size()) {
                            PRtoNU[newPR] = inst.dest.NU;
                        }
                    }
                } catch (...) {
                    if(debugLevel > 0) {
                        cout << "//Error in register allocation for dest" << endl;
                    }
                    // Use a safe fallback
                    inst.dest.PR = 1;
                }
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
