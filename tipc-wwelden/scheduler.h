// scheduler.h

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "internalRep.h"
#include "scanner.h"
#include "parser.h"

using namespace std;

// Instruction latencies for ILOC operations
const unordered_map<OpCode, int> LATENCIES = {
    {LOAD, 3},
    {LOADI, 1},
    {STORE, 3},
    {ADD, 1},
    {SUB, 1},
    {MULT, 2},
    {LSHIFT, 1},
    {RSHIFT, 1},
    {OUTPUT, 1},
    {NOP, 1}
};

// Structure to represent a node in the dependency graph
struct DepNode {
    int label;                                  // Instruction label (n1, n2, etc.)
    Line instruction;                           // The ILOC instruction
    unordered_set<int> dependencies;            // Set of instruction labels this node depends on
    int weight;                                 // Latency-weighted distance to root
};

class Scheduler {
public:
    // Constructor
    Scheduler();

    // Main scheduling functions
    void buildDependencyGraph(vector<Line>& block);
    void computeWeights();

    // Printing functions
    void printGraph();

    // Data members
    vector<DepNode> nodes;
    unordered_map<int, unordered_set<int>> edges;
    unordered_map<int, int> weights;

private:
    // Helper functions
    void addRegisterDependencies(vector<DepNode>& labeledInstructions);
    void addSerializationDependencies(vector<DepNode>& labeledInstructions);
    void findRoots();

    // Track the most recent memory-related instructions
    int lastOutput;
    int lastStore;
    vector<int> lastLoads;

    // List of root nodes (no incoming edges)
    vector<int> roots;
};

#endif
