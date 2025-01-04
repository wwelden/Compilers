#pragma once
#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <vector>
#include <stack>
#include "compilerUtils.h"
#include "scanner.h"
#include "parser.h"


using namespace std;

class Allocator {
    public:
            Allocator(int k);
            void ComputeLastUse(std::vector<Line> &Block);
            void LocalRegAllocation(std::vector<Line> &Block);
            void LocalRegisterAllocation(std::vector<Line> &Block);
            int getInfinity();
            int numPRs;// this should be set equal to k ?
            vector<Line> allocatedBlock;
    private:
            void Update(Operand &instruction, int index);
            void restore(int vr);
            void printTables();
            // int getPR(const Operand& inst);
            int biggestReg(std::vector<Line> &Block);
            int getPR(Line &inst);
            int getNextSpillOffset();
            string MemoryLocation(int offset);
            int getRestoreOffset(int VR);
            int maxReg(std::vector<Line> &Block);
            int spill();
            int GetPR();
            int main();
            vector<int> SRtoVR;
            vector<int> LastUse;
            int VRName;
            vector<int> VRtoPR;
            vector<int> PRtoVR;
            vector<int> PRtoNU;
            stack<int> FreePRs;
            vector<int> VRtoMemAd;
            void handleNU(Operand &op);
            void ensurePR(Operand &op);
};
