#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

class Graph {
private:
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list representation

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    vector<int> getSuccessors(int node) {
        return adj[node];
    }

    vector<int> getPredecessors(int node) {
        vector<int> predecessors;
        for (int i = 0; i < V; i++) {
            for (int neighbor : adj[i]) {
                if (neighbor == node) {
                    predecessors.push_back(i);
                }
            }
        }
        return predecessors;
    }

    bool hasBackEdge(int start, unordered_set<int>& nset) {
        vector<int> successors = getSuccessors(start);
        for (int succ : successors) {
            // Check if successor is part of the loop region and points back to start
            if (nset.find(succ) != nset.end() && succ == start) {
                return true; // Found a back edge
            }
        }
        return false; // No back edge
    }

    string Cyclic_Region_Type(int node, unordered_set<int>& nset) {
        // Check for SelfLoop
        if (nset.size() == 1 && adj[node].size() > 0 && adj[node][0] == node) {
            return "SelfLoop";
        }

        // Check for WhileLoop
        if (hasBackEdge(node, nset)) {
            vector<int> successors = getSuccessors(node);
            if (successors.size() == 1) {
                int loopBody = successors[0];
                vector<int> loopSuccessors = getSuccessors(loopBody);

                if (loopSuccessors.size() == 1 && loopSuccessors[0] == node) {
                    return "WhileLoop";
                }
            }
        }

        // Default to NaturalLoop if no structure matches
        return "NaturalLoop";
    }
};

// Test Cases
void runTests() {
    cout << "Test Case 1: WhileLoop with back edge" << endl;
    Graph g1(2);
    g1.addEdge(0, 1); // Entry -> Body
    g1.addEdge(1, 0); // Body -> Entry (back edge)
    unordered_set<int> nset1 = {0, 1};
    cout << "Region Type: " << g1.Cyclic_Region_Type(0, nset1) << endl;
    // Expected output: WhileLoop

    cout << "\nTest Case 2: NaturalLoop without strict structure" << endl;
    Graph g2(3);
    g2.addEdge(0, 1); // Entry -> Body
    g2.addEdge(1, 2); // Body -> Another node
    g2.addEdge(2, 0); // Back edge to Entry
    unordered_set<int> nset2 = {0, 1, 2};
    cout << "Region Type: " << g2.Cyclic_Region_Type(0, nset2) << endl;
    // Expected output: NaturalLoop

    cout << "\nTest Case 3: No loop structure" << endl;
    Graph g3(3);
    g3.addEdge(0, 1); // Entry -> Body
    g3.addEdge(1, 2); // Body -> Exit
    unordered_set<int> nset3 = {0, 1, 2};
    cout << "Region Type: " << g3.Cyclic_Region_Type(0, nset3) << endl;
    // Expected output: NaturalLoop
}

int main() {
    runTests();
    return 0;
}