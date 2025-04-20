#include <iostream>
#include <vector>

using namespace std;

class Graph {
private:
    int V; // Graph vertices
    vector<vector<int>> adj; // Adjacency list
    int PostMax; // Current postorder index
    vector<int> Post; // Stores postorder traversal result

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Add a directed edge u -> v
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // Perform DFS and compute postorder traversal
    void DFS_Postorder(int u, vector<bool>& visited) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                DFS_Postorder(v, visited);
            }
        }
        Post.push_back(u); // Add current node to postorder
    }

    // Get postorder traversal starting from `start`
    vector<int> getPostorder(int start) {
        Post.clear(); // Reset postorder array
        vector<bool> visited(V, false); // Reset visited array
        DFS_Postorder(start, visited); // Perform DFS traversal
        return Post;
    }

    // Print postorder traversal
    void printPostorder(int start) {
        vector<int> postorder = getPostorder(start);
        cout << "Postorder Traversal: ";
        for (int node : postorder) {
            cout << node << " ";
        }
        cout << endl;
    }
};

// Test cases
void runTests() {
    // Test Case 1: Simple Graph
    cout << "Test Case 1: Simple Graph" << endl;
    Graph g1(5);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(1, 3);
    g1.addEdge(3, 4);
    g1.printPostorder(0);
    // Expected output: 4 3 2 1 0
    // or
    // Expected output: 2 4 3 1 0

    // Test Case 2: Cyclic Graph
    cout << "\nTest Case 2: Cyclic Graph" << endl;
    Graph g2(4);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(2, 0); // Cycle
    g2.addEdge(2, 3);
    g2.printPostorder(0);
    // Expected output: 3 2 1 0

    // Test Case 3: Disconnected Graph
    cout << "\nTest Case 3: Disconnected Graph" << endl;
    Graph g3(6);
    g3.addEdge(0, 1);
    g3.addEdge(2, 3);
    g3.addEdge(4, 5);
    g3.printPostorder(0); // Start from node 0
    // Expected output: 1 0

    vector<int> postorder2 = g3.getPostorder(2);
    cout << "Postorder Traversal from Node 2: ";
    for (int node : postorder2) {
        cout << node << " ";
    }
    cout << endl;
    // Expected output: 3 2

    vector<int> postorder3 = g3.getPostorder(4);
    cout << "Postorder Traversal from Node 4: ";
    for (int node : postorder3) {
        cout << node << " ";
    }
    cout << endl;
    // Expected output: 5 4
}

int main() {
    runTests();
    return 0;
}