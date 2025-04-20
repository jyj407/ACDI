#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
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

    bool hasPath(int start, int target, unordered_set<int>& visited) {
        if (start == target) return true;
        visited.insert(start);
        for (int neighbor : adj[start]) {
            if (visited.find(neighbor) == visited.end()) {
                if (hasPath(neighbor, target, visited)) {
                    return true;
                }
            }
        }
        return false;
    }

    string Cyclic_Region_Type(int node, unordered_set<int>& nset) {
        // Check for SelfLoop
        if (nset.size() == 1 && adj[node].size() > 0 && adj[node][0] == node) {
            return "SelfLoop";
        }

        // Check for Improper
        for (int m : nset) {
            unordered_set<int> visited;
            if (!hasPath(node, m, visited)) {
                return "Improper";
            }
        }

        // Check for WhileLoop
        vector<int> successors = getSuccessors(node);
        if (successors.size() == 2) {
            int m = successors[0];
            int n = successors[1];
            vector<int> succM = getSuccessors(m);
            vector<int> succN = getSuccessors(n);
            vector<int> predM = getPredecessors(m);
            vector<int> predN = getPredecessors(n);

            if (succM == succN && predM.size() == 1 && predN.size() == 1) {
                return "WhileLoop";
            }
        }

        // Default to NaturalLoop
        return "NaturalLoop";
    }
};

// Test Cases
void runTests() {
    cout << "Test Case 1: SelfLoop" << endl;
    Graph g1(3);
    g1.addEdge(0, 0); // Node 0 points to itself
    unordered_set<int> nset1 = {0};
    cout << "Region Type: " << g1.Cyclic_Region_Type(0, nset1) << endl;
    // Expected output: SelfLoop

    cout << "\nTest Case 2: Improper" << endl;
    Graph g2(4);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(3, 2); // Node 3 is not reachable from node 0
    unordered_set<int> nset2 = {0, 1, 2, 3};
    cout << "Region Type: " << g2.Cyclic_Region_Type(0, nset2) << endl;
    // Expected output: Improper

    cout << "\nTest Case 3: WhileLoop" << endl;
    Graph g3(4);
    g3.addEdge(0, 1);
    g3.addEdge(0, 2);
    g3.addEdge(1, 3);
    g3.addEdge(2, 3); // Node 1 and 2 both point to 3
    unordered_set<int> nset3 = {0, 1, 2, 3};
    cout << "Region Type: " << g3.Cyclic_Region_Type(0, nset3) << endl;
    // Expected output: WhileLoop

    cout << "\nTest Case 4: NaturalLoop" << endl;
    Graph g4(5);
    g4.addEdge(0, 1);
    g4.addEdge(1, 2);
    g4.addEdge(2, 0); // Nodes form a natural cyclic loop
    unordered_set<int> nset4 = {0, 1, 2};
    cout << "Region Type: " << g4.Cyclic_Region_Type(0, nset4) << endl;
    // Expected output: NaturalLoop
}

int main() {
    runTests();
    return 0;
}