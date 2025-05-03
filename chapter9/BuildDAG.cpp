#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

using namespace std;

struct Instruction {
    int id;
    string name;
    vector<int> dependencies; // Instructions this one depends on
};

struct DAG {
    set<int> nodes;
    map<int, set<int>> edges; // Dependency edges
    set<int> roots;
    map<pair<int, int>, int> latency; // Latency between instructions
};

// Function to detect dependency conflicts
bool Conflict(const Instruction& inst1, const Instruction& inst2) {
    // Check if inst2 depends on inst1
    return find(inst2.dependencies.begin(), inst2.dependencies.end(), inst1.id) != inst2.dependencies.end();
}

// Function to compute latency (simplified)
int Latency(const Instruction& inst1, const Instruction& inst2) {
    return 2; // Example fixed latency
}

// Function to build the DAG for scheduling
DAG Build_DAG(const vector<Instruction>& instructions) {
    DAG dag;
    
    for (const auto& inst : instructions) {
        dag.nodes.insert(inst.id);
        set<int> conflicts;
        
        for (const auto& prev_inst : instructions) {
            // Good range for loop way to avoid using i, j index to only check j < i.
            if (prev_inst.id == inst.id) break; // Only check prior instructions
            
            if (Conflict(prev_inst, inst)) {
                conflicts.insert(prev_inst.id);
            }
        }

        if (conflicts.empty()) {
            dag.roots.insert(inst.id);
        } else {
            for (int dep : conflicts) {
                dag.edges[dep].insert(inst.id); // save successors to adjacent list edges
                dag.latency[{dep, inst.id}] = Latency(instructions[dep], inst);
            }
        }
    }
    
    return dag;
}

// Function to print the DAG
void Print_DAG(const DAG& dag) {
    cout << "DAG Nodes: ";
    for (int node : dag.nodes) cout << node << " ";
    cout << "\nRoot Nodes: ";
    for (int root : dag.roots) cout << root << " ";
    cout << "\nDependencies:\n";
    
    for (const auto& edge : dag.edges) {
        cout << "Inst " << edge.first << " -> { ";
        for (int dep : edge.second) cout << dep << " ";
        cout << "}\n";
    }
    
    cout << "\nLatencies:\n";
    for (const auto& lat : dag.latency) {
        cout << "Latency(" << lat.first.first << " → " << lat.first.second << ") = " << lat.second << "\n";
    }
}

// **Test Example**
int main() {
    vector<Instruction> instructions = {
        {1, "LOAD", {}},           // No dependencies (root node)
        {2, "ADD", {1}},           // Depends on LOAD
        {3, "MUL", {2}},           // Depends on ADD
        {4, "STORE", {3}}          // Depends on MUL
    };

    DAG dag = Build_DAG(instructions);
    Print_DAG(dag);

    return 0;
}