#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>

using namespace std;

// Helper function to check if there's a path from `start` to `target` in `graph`
bool hasPath(int start, int target, const unordered_map<int, unordered_set<int>>& graph, unordered_set<int> excluded = {}) {
    queue<int> q;
    unordered_set<int> visited;
    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == target) return true;

        for (int neighbor : graph.at(current)) {
            if (visited.count(neighbor) == 0 && excluded.count(neighbor) == 0) {
                q.push(neighbor);
                visited.insert(neighbor);
            }
        }
    }
    return false;
}

// Function to compute Multiple Entry Conditions (MEC)
unordered_set<int> MEC_Entries(int node, const unordered_set<int>& nset, const unordered_map<int, unordered_set<int>>& graph) {
    unordered_set<int> entries;

    for (int n : nset) {
        for (int pred : graph.at(n)) {
            if (nset.count(pred) == 0) { // If a predecessor is outside the nset
                entries.insert(n);
            }
        }
    }

    return entries;
}

// Function to compute the Nearest Common Dominator (NCD)
int NC_Domin(const unordered_set<int>& entryNodes, const unordered_set<int>& nodes, const unordered_map<int, unordered_set<int>>& graph) {
    // For simplicity, return the first entryNode as the dominator in this demo
    // In real applications, this would compute the true nearest dominator
    if (!entryNodes.empty()) {
        return *entryNodes.begin();
    }
    return -1; // No valid dominator found
}

// Minimize_Improper Algorithm
unordered_set<int> Minimize_Improper(const unordered_set<int>& N, const vector<pair<int, int>>& E, int node, unordered_set<int> nset) {
    // Build adjacency list representation of the graph
    unordered_map<int, unordered_set<int>> graph;
    for (const auto& edge : E) {
        graph[edge.first].insert(edge.second);
    }

    // Initialize I with MEC_Entries
    unordered_set<int> I = MEC_Entries(node, nset, graph);

    // Compute the Nearest Common Dominator (NCD)
    int ncd = NC_Domin(I, N, graph);

    // Iterate over all nodes in N except ncd
    for (int n : N) {
        if (n == ncd) continue;

        // Check Path Condition: Path from ncd to n
        bool pathFromNcd = hasPath(ncd, n, graph);

        // Check MEC Condition: Path from n to some node in I excluding ncd
        bool pathToI = false;
        for (int m : I) {
            if (hasPath(n, m, graph, {ncd})) {
                pathToI = true;
                break;
            }
        }

        // Add n to I if both conditions are true
        if (pathFromNcd && pathToI) {
            I.insert(n);
        }
    }

    // Add ncd to I and return
    I.insert(ncd);
    return I;
}

// Test Cases
void runTests() {
    cout << "Test Case 1: Simple Graph" << endl;

    unordered_set<int> N = {0, 1, 2, 3, 4};
    vector<pair<int, int>> E = {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 1}, // Back edge forming a cycle
        {2, 4}
    };
    int node = 0;
    unordered_set<int> nset = {1, 2, 3};

    unordered_set<int> result = Minimize_Improper(N, E, node, nset);

    cout << "Result: ";
    for (int n : result) {
        cout << n << " ";
    }
    cout << endl;

    cout << "\nTest Case 2: Another Graph" << endl;

    N = {0, 1, 2, 3, 4, 5};
    E = {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 3} // Back edge forming a cycle
    };
    node = 0;
    nset = {3, 4, 5};

    result = Minimize_Improper(N, E, node, nset);

    cout << "Result: ";
    for (int n : result) {
        cout << n << " ";
    }
    cout << endl;
}

int main() {
    runTests();
    return 0;
}