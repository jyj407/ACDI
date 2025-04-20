#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <algorithm>
using namespace std;

class Graph {
private:
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    void DFS_Postorder(int u, vector<int>& visited, vector<int>& postorder) {
        visited[u] = 1;
        for (int v : adj[u]) {
            if (!visited[v]) {
                DFS_Postorder(v, visited, postorder);
            }
        }
        postorder.push_back(u);
    }

    bool Path_Back(int start, int target, vector<int>& visited) {
        if (start == target) return true;
        visited[start] = 1;
        for (int v : adj[start]) {
            if (!visited[v] && Path_Back(v, target, visited)) {
                return true;
            }
        }
        return false;
    }

    void ReduceRegion(unordered_set<int>& region, int representative) {
        vector<vector<int>> new_adj(V);

        for (int u = 0; u < V; ++u) {
            for (int v : adj[u]) {
                if (region.find(u) == region.end() && region.find(v) == region.end()) {
                    new_adj[u].push_back(v);
                } else if (region.find(v) != region.end()) {
                    new_adj[u].push_back(representative);
                }
            }
        }

        // Clear the adjacency list for nodes in the region
        for (int node : region) {
            adj[node].clear();
        }

        adj = new_adj;
    }

    void StructuralAnalysis(int entry) {
        vector<int> visited(V, 0), postorder;
        unordered_set<int> region;

        // Perform DFS to calculate postorder traversal
        DFS_Postorder(entry, visited, postorder);
        reverse(postorder.begin(), postorder.end());

        // Analyze and reduce regions
        for (int node : postorder) {
            visited.assign(V, 0);
            region.clear();

            // Check for cyclic region (Path_Back) and reduce it
            if (Path_Back(node, node, visited)) {
                region.insert(node);
                for (int v : adj[node]) {
                    visited.assign(V, 0);
                    if (Path_Back(v, node, visited)) {
                        region.insert(v);
                    }
                }
                ReduceRegion(region, node);
            }
        }
    }

    void PrintGraph() {
        cout << "Graph:" << endl;
        for (int u = 0; u < V; ++u) {
            cout << u << " -> ";
            for (int v : adj[u]) {
                cout << v << " ";
            }
            cout << endl;
        }
    }
};

// Test cases
void RunTests() {
    cout << "Test Case 1: Acyclic Graph" << endl;
    Graph g1(5);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);
    g1.addEdge(3, 4);
    g1.StructuralAnalysis(0);
    g1.PrintGraph();

    cout << "\nTest Case 2: Cyclic Graph" << endl;
    Graph g2(4);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(2, 0);
    g2.addEdge(2, 3);
    g2.StructuralAnalysis(0);
    g2.PrintGraph();

    cout << "\nTest Case 3: Disconnected Graph" << endl;
    Graph g3(6);
    g3.addEdge(0, 1);
    g3.addEdge(2, 3);
    g3.addEdge(4, 5);
    g3.StructuralAnalysis(0);
    g3.PrintGraph();
}

int main() {
    RunTests();
    return 0;
}
