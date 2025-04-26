#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <set>
using namespace std;

// Helper types
typedef unordered_set<string> DefinitionSet; // Set of definitions
typedef unordered_map<int, DefinitionSet> NodeDefinitions; // Definitions for each node

class Solution {
public:
    void ReachingDefinitions(
        vector<int>& nodes,                          // List of nodes (program points)
        int entry,                                   // Entry point
        unordered_map<int, vector<int>>& edges,     // CFG edges (Control Flow Graph)
        unordered_map<int, DefinitionSet>& gen,     // GEN sets (generated definitions)
        unordered_map<int, DefinitionSet>& kill     // KILL sets (definitions killed)
    ) {
        // Step 1: Create the predecessor map
        unordered_map<int, vector<int>> predecessors;
        for (const auto& [node, successors] : edges) {
            for (int successor : successors) {
                predecessors[successor].push_back(node);
            }
        }

        // Step 2: Initialize the OUT set (definitions reaching each node)
        NodeDefinitions outSet;
        for (int node : nodes) {
            outSet[node] = {}; // Start with empty set
        }

        // Initialize the entry node with its GEN set
        outSet[entry] = gen[entry];

        // Step 3: Initialize the worklist with all nodes except the entry
        queue<int> worklist;
        for (int node : nodes) {
            if (node != entry) {
                worklist.push(node);
            }
        }

        // Step 4: Main loop
        while (!worklist.empty()) {
            int current = worklist.front();
            worklist.pop();

            // Collect predecessor values and combine them
            DefinitionSet inSet;
            for (int predecessor : predecessors[current]) {
                inSet.insert(outSet[predecessor].begin(), outSet[predecessor].end());
            }

            // Compute the OUT set for the current node: (IN - KILL) ∪ GEN
            DefinitionSet newOutSet = inSet;
            for (const string& def : kill[current]) {
                newOutSet.erase(def);
            }
            newOutSet.insert(gen[current].begin(), gen[current].end());

            // Check if the OUT set changed
            if (newOutSet != outSet[current]) {
                outSet[current] = newOutSet;

                // Add successors to the worklist
                for (int successor : edges[current]) {
                    worklist.push(successor);
                }
            }
        }

        // Output results
        cout << "Final Reaching Definitions:" << endl;
        for (int node : nodes) {
            cout << "Node " << node << ": {";
            for (const auto& def : outSet[node]) {
                cout << def << " ";
            }
            cout << "}" << endl;
        }
    }
};

// Test the implementation
int main() {
    Solution solution;

    // Example Control Flow Graph (CFG)
    vector<int> nodes = {1, 2, 3, 4};
    int entry = 1;
    unordered_map<int, vector<int>> edges = {
        {1, {2, 3}},   // Node 1 goes to 2 and 3
        {2, {4}},      // Node 2 goes to 4
        {3, {4}},      // Node 3 goes to 4
        {4, {}}        // Node 4 has no successors
    };

    // Example GEN and KILL sets
    unordered_map<int, DefinitionSet> gen = {
        {1, {"x=1"}},  // Node 1 generates definition x=1
        {2, {"y=2"}},  // Node 2 generates definition y=2
        {3, {"x=3"}},  // Node 3 generates definition x=3
        {4, {"z=4"}}   // Node 4 generates definition z=4
    };
    unordered_map<int, DefinitionSet> kill = {
        {1, {}},       // Node 1 kills nothing
        {2, {"x=1"}},  // Node 2 kills definition x=1
        {3, {"x=1"}},  // Node 3 kills definition x=1
        {4, {"y=2", "x=3"}} // Node 4 kills y=2 and x=3
    };

    // Run the Reaching Definitions algorithm
    solution.ReachingDefinitions(nodes, entry, edges, gen, kill);

    return 0;
}