#include <iostream>
#include <iomanip>
#include <queue>
#include <algorithm>
#include "scheduler.h"

using namespace std;

Scheduler::Scheduler() {
    lastOutput = -1;
    lastStore = -1;
}

void Scheduler::buildDependencyGraph(vector<Line>& block) {
    // Filter out nop instructions
    vector<Line> filteredBlock;
    for (const Line& line : block) {
        if (line.opcode != NOP) {
            filteredBlock.push_back(line);
        }
    }

    // Create nodes with instruction labels starting at 1
    nodes.clear();
    edges.clear();
    for (int i = 0; i < filteredBlock.size(); i++) {
        DepNode node;
        node.label = i + 1;  // Labels start at 1 (n1, n2, etc.)
        node.instruction = filteredBlock[i];
        node.weight = 0;     // Will be computed later
        nodes.push_back(node);

        // Initialize empty set of dependencies for this node
        edges[node.label] = unordered_set<int>();
    }

    // Add register dependencies
    addRegisterDependencies(nodes);

    // Add serialization dependencies
    addSerializationDependencies(nodes);
}

void Scheduler::addRegisterDependencies(vector<DepNode>& labeledInstructions) {
    // Map to track which instruction defines each register
    unordered_map<int, int> defMap;  // Maps SR to instruction label that defines it

    for (DepNode& node : labeledInstructions) {
        Line& instr = node.instruction;

        // Check if this instruction uses registers that were defined by previous instructions
        if (instr.opcode == LOAD || instr.opcode == LOADI) {
            // For LOAD, op1 is a source register
            if (instr.opcode == LOAD && instr.op1.isReg && instr.op1.SR >= 0 && defMap.count(instr.op1.SR)) {
                // Current instruction (node.label) depends on the instruction that defined op1.SR
                edges[node.label].insert(defMap[instr.op1.SR]);
            }

            // dest is defined by this instruction
            if (instr.dest.SR >= 0) {
                defMap[instr.dest.SR] = node.label;
            }
        }
        else if (instr.opcode == STORE) {
            // For STORE, op1 is a source register for the value to store
            if (instr.op1.isReg && instr.op1.SR >= 0 && defMap.count(instr.op1.SR)) {
                // Current instruction depends on the instruction that defined op1.SR
                edges[node.label].insert(defMap[instr.op1.SR]);
            }

            // op2 is a source register for the memory address
            if (instr.op2.isReg && instr.op2.SR >= 0 && defMap.count(instr.op2.SR)) {
                // Current instruction depends on the instruction that defined op2.SR
                edges[node.label].insert(defMap[instr.op2.SR]);
            }
        }
        else if (instr.opcode == ADD || instr.opcode == SUB || instr.opcode == MULT ||
                 instr.opcode == LSHIFT || instr.opcode == RSHIFT) {
            // For binary operations, op1 and op2 are source registers
            if (instr.op1.isReg && instr.op1.SR >= 0 && defMap.count(instr.op1.SR)) {
                // Current instruction depends on the instruction that defined op1.SR
                edges[node.label].insert(defMap[instr.op1.SR]);
            }

            if (instr.op2.isReg && instr.op2.SR >= 0 && defMap.count(instr.op2.SR)) {
                // Current instruction depends on the instruction that defined op2.SR
                edges[node.label].insert(defMap[instr.op2.SR]);
            }

            // dest is defined by this instruction
            if (instr.dest.SR >= 0) {
                defMap[instr.dest.SR] = node.label;
            }
        }
        else if (instr.opcode == OUTPUT) {
            // For OUTPUT, op1 is a source register if it's a register
            if (instr.op1.isReg && instr.op1.SR >= 0 && defMap.count(instr.op1.SR)) {
                // Current instruction depends on the instruction that defined op1.SR
                edges[node.label].insert(defMap[instr.op1.SR]);
            }
        }
    }
}

void Scheduler::addSerializationDependencies(vector<DepNode>& labeledInstructions) {
    lastOutput = -1;
    lastStore = -1;
    lastLoads.clear();

    for (DepNode& node : labeledInstructions) {
        Line& instr = node.instruction;
        int label = node.label;

        if (instr.opcode == OUTPUT) {
            // 1. Output depends on the most recent output (values must be output in order)
            if (lastOutput != -1) {
                edges[label].insert(lastOutput);
            }

            // 2. Output depends on the most recent store (may modify the memory location)
            if (lastStore != -1) {
                edges[label].insert(lastStore);
            }

            lastOutput = label;
        }
        else if (instr.opcode == LOAD) {
            // Load depends on the most recent store (may modify the memory location)
            if (lastStore != -1) {
                edges[label].insert(lastStore);
            }

            lastLoads.push_back(label);
        }
        else if (instr.opcode == STORE) {
            // Store depends on:

            // 1. The most recent output instruction
            if (lastOutput != -1) {
                edges[label].insert(lastOutput);
            }

            // 2. Every previous load instruction (may access the same memory location)
            // Optimization: only include loads after the most recent store
            for (int loadLabel : lastLoads) {
                edges[label].insert(loadLabel);
            }

            lastStore = label;
            lastLoads.clear();  // Optimization: Clear loads list after a store
        }
    }
}

void Scheduler::findRoots() {
    roots.clear();

    // Create a set of all nodes that have incoming edges
    unordered_set<int> hasIncoming;
    for (const auto& edge : edges) {
        for (int dep : edge.second) {
            hasIncoming.insert(dep);
        }
    }

    // Nodes that don't have incoming edges are roots
    for (const DepNode& node : nodes) {
        if (hasIncoming.find(node.label) == hasIncoming.end()) {
            roots.push_back(node.label);
        }
    }
}

void Scheduler::computeWeights() {
    // 1. Construct a reverse graph where edges point from parent to child
    unordered_map<int, unordered_set<int>> reverseGraph;
    for (const auto& edgePair : edges) {
        int child = edgePair.first;
        for (int parent : edgePair.second) {
            reverseGraph[parent].insert(child);
        }
    }

    // 2. Find the nodes with no dependencies (root nodes in the dependency graph)
    vector<int> roots;
    for (const DepNode& node : nodes) {
        if (edges[node.label].empty()) {
            roots.push_back(node.label);
        }
    }

    // 3. Initialize weights map
    weights.clear();
    for (const DepNode& node : nodes) {
        int label = node.label;
        OpCode opcode = node.instruction.opcode;
        weights[label] = 0; // Start all weights at 0, except roots
    }

    // 4. Initialize root weights to their latencies
    for (int rootLabel : roots) {
        OpCode opcode;
        for (const DepNode& node : nodes) {
            if (node.label == rootLabel) {
                opcode = node.instruction.opcode;
                break;
            }
        }
        weights[rootLabel] = LATENCIES.find(opcode)->second;
    }

    // 5. Use a queue to propagate weights from roots to leaves
    queue<int> q;
    for (int rootLabel : roots) {
        q.push(rootLabel);
    }

    // 6. Process the queue to propagate weights
    unordered_set<int> processed;
    int iterations = 0;
    while (!q.empty() && iterations < 100) { // Prevent infinite loops
        iterations++;
        int currentLabel = q.front();
        q.pop();

        processed.insert(currentLabel);

        // Process all children
        if (reverseGraph.find(currentLabel) != reverseGraph.end()) {
            for (int childLabel : reverseGraph[currentLabel]) {
                // Get the child's latency
                OpCode childOpcode;
                for (const DepNode& node : nodes) {
                    if (node.label == childLabel) {
                        childOpcode = node.instruction.opcode;
                        break;
                    }
                }
                int childLatency = LATENCIES.find(childOpcode)->second;

                // Calculate new weight: parent weight + child latency
                int newWeight = weights[currentLabel] + childLatency;

                // Update child's weight if the new weight is higher
                if (newWeight > weights[childLabel]) {
                    weights[childLabel] = newWeight;

                    // Only add to queue if not already processed in this iteration
                    if (processed.find(childLabel) == processed.end()) {
                        q.push(childLabel); // Re-process this child
                    }
                }
            }
        }
    }

    // 7. Update node weights
    for (DepNode& node : nodes) {
        node.weight = weights[node.label];
    }
}

void Scheduler::printGraph() {
    // Print nodes section
    cout << "nodes:" << endl << endl;
    for (const DepNode& node : nodes) {
        cout << "    n" << node.label << " : ";

        // Print the instruction with its source registers
        Line instr = node.instruction;
        switch (instr.opcode) {
            case LOAD:
                cout << "load ";
                if (instr.op1.isReg) {
                    cout << "r" << instr.op1.SR;
                } else {
                    cout << instr.op1.constant;
                }
                cout << " => r" << instr.dest.SR;
                break;

            case LOADI:
                cout << "loadI " << instr.op1.constant << " => r" << instr.dest.SR;
                break;

            case STORE:
                cout << "store ";
                if (instr.op1.isReg) {
                    cout << "r" << instr.op1.SR;
                } else {
                    cout << instr.op1.constant;
                }
                cout << " => ";
                if (instr.op2.isReg) {
                    cout << "r" << instr.op2.SR;
                } else {
                    cout << instr.op2.constant;
                }
                break;

            case ADD:
                cout << "add r" << instr.op1.SR << ", r" << instr.op2.SR << " => r" << instr.dest.SR;
                break;

            case SUB:
                cout << "sub r" << instr.op1.SR << ", r" << instr.op2.SR << " => r" << instr.dest.SR;
                break;

            case MULT:
                cout << "mult r" << instr.op1.SR << ", r" << instr.op2.SR << " => r" << instr.dest.SR;
                break;

            case LSHIFT:
                cout << "lshift r" << instr.op1.SR << ", r" << instr.op2.SR << " => r" << instr.dest.SR;
                break;

            case RSHIFT:
                cout << "rshift r" << instr.op1.SR << ", r" << instr.op2.SR << " => r" << instr.dest.SR;
                break;

            case OUTPUT:
                cout << "output ";
                if (instr.op1.isReg) {
                    cout << "r" << instr.op1.SR;
                } else {
                    cout << instr.op1.constant;
                }
                break;

            default:
                cout << "unknown instruction";
        }
        cout << endl;
    }

    // Print edges section
    cout << endl << "edges:" << endl << endl;

    // Create a reverse mapping of dependencies to match the example format
    // In the example, if n1 depends on n2, it shows: n1 : { n2 }
    // Which means n1 must be executed after n2
    for (const DepNode& node : nodes) {
        cout << "    n" << node.label << " : { ";

        // Get the dependencies for this node (nodes that must complete before this one)
        auto deps = edges[node.label];
        if (!deps.empty()) {
            bool first = true;
            for (int dep : deps) {
                if (!first) {
                    cout << ", ";
                }
                cout << "n" << dep;
                first = false;
            }
        }

        cout << " }" << endl;
    }

    // Print weights section
    cout << endl << "weights:" << endl << endl;
    for (const DepNode& node : nodes) {
        cout << "    n" << node.label << " : " << node.weight << endl;
    }
}
