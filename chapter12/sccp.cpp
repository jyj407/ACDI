#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <set>

using namespace std;

// Hash function for pair<int, int>
struct pair_hash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const {
        return hash<T1>()(p.first) ^ hash<T2>()(p.second);
    }
};

// Lattice values for constants
enum LatticeVal { TOP, CONSTANT, BOTTOM };

struct Inst {
    string kind;   // Instruction type (binexp, unexp, phi)
    int left, right;
    LatticeVal value;

    Inst(string k, int l = -1, int r = -1) : kind(k), left(l), right(r), value(TOP) {}
};

// Flow and SSA worklists
set<pair<int, int>> FlowWL;  // Using set instead of unordered_set
set<pair<int, int>> SSAWL;

// Lattice cells (constant propagation tracking)
unordered_map<int, LatticeVal> LatCell;
unordered_map<pair<int, int>, bool, pair_hash> ExecFlag; // Fixed ExecFlag
unordered_map<int, vector<int>> FlowGraph;
unordered_map<int, vector<int>> SSAGraph;

// Function to evaluate lattice value
LatticeVal Lat_Eval(const Inst &inst) {
    if (inst.kind == "binexp" && LatCell[inst.left] == CONSTANT && LatCell[inst.right] == CONSTANT) {
        return CONSTANT; // Simplification: assuming fixed operations
    }
    return BOTTOM;
}

// SCCP main function
void Sparse_Cond_Const(int entry, unordered_map<int, Inst> &InstMap) {
    // Initialize worklists
    for (const auto &[key, val] : FlowGraph) {
        ExecFlag[{key, key}] = false; // Use pair<int, int>
    }

    // Add initial edges from entry
    for (int succ : FlowGraph[entry]) {
        FlowWL.insert({entry, succ});
    }

    // Process FlowWL and SSAWL
    while (!FlowWL.empty() || !SSAWL.empty()) {
        // Process flow edges
        if (!FlowWL.empty()) {
            auto it = FlowWL.begin();
            pair<int, int> edge = *it;
            FlowWL.erase(it);

            int a = edge.first, b = edge.second;
            if (!ExecFlag[{a, b}]) {
                ExecFlag[{a, b}] = true;

                if (InstMap[b].kind == "phi") {
                    LatCell[InstMap[b].left] = LatCell[InstMap[b].right];
                } else {
                    LatCell[b] = Lat_Eval(InstMap[b]);
                    if (LatCell[b] == CONSTANT) {
                        SSAWL.insert({b, b});
                    }
                }
            }
        }

        // Process SSA edges
        if (!SSAWL.empty()) {
            auto it = SSAWL.begin();
            pair<int, int> edge = *it;
            SSAWL.erase(it);

            int a = edge.first, b = edge.second;
            LatCell[b] = Lat_Eval(InstMap[b]);
        }
    }
}

// Example usage
int main() {
    // Define instructions
    unordered_map<int, Inst> InstMap;
    InstMap[1] = Inst("binexp", 2, 3);
    InstMap[2] = Inst("constant");
    InstMap[3] = Inst("constant");

    // Define control flow and SSA edges
    FlowGraph[1] = {4, 5};
    FlowGraph[4] = {6};
    SSAGraph[5] = {6};

    // Initialize lattice values
    LatCell[2] = CONSTANT;
    LatCell[3] = CONSTANT;

    // Run SCCP
    Sparse_Cond_Const(1, InstMap);

    // Output results
    cout << "Final Lattice Values:" << endl;
    for (const auto &[key, val] : LatCell) {
        cout << "Inst " << key << ": " << (val == CONSTANT ? "CONSTANT" : "BOTTOM") << endl;
    }

    return 0;
}
