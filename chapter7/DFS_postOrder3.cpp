#include <iostream>
#include <vector>

using namespace std;

class Graph {
private:
    int V;                // Number of vertices in the graph
    vector<vector<int>> adj; // Adjacency list representation of the graph
    int PostMax;          // Current index for postorder numbering
    vector<int> Post;     // Array to store the postorder traversal result

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
        Post.resize(V, -1);  // Initialize the Post array with default values
        PostMax = -1;        // Start numbering from -1
    }

    int getV() {
        return V;
    }

    // Add an edge from u to v
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // Depth-First Search to calculate postorder traversal
    void DFS_Postorder(int u, vector<bool>& visited) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                DFS_Postorder(v, visited);
            }
        }
        PostMax += 1;          // Increment postorder index
        Post[PostMax] = u;     // Assign the current node to its postorder position
    }

    // Get postorder traversal starting from a specific node
    vector<int> getPostorder(int start) {
        Post.assign(V, -1);        // Reset Post array to default values
        PostMax = -1;              // Reset the postorder numbering
        vector<bool> visited(V, false); // Mark all nodes as unvisited
        DFS_Postorder(start, visited); // Perform DFS from the start node
        return Post;
    }

    // Print postorder traversal results
    void printPostorder(int start) {
        vector<int> postorder = getPostorder(start);
        cout << "Postorder Traversal: ";
        for (int i = 0; i <= PostMax; i++) { // Only print valid indices
            cout << postorder[i] << " ";
        }
        cout << endl;
    }
};

// Test cases
void runTests() {
    // Test Case 1: Linear Graph
    cout << "Test Case 1: Linear Graph" << endl;
    Graph g1(5);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);
    g1.addEdge(3, 4);
    g1.printPostorder(0);
    // Expected output: 4 3 2 1 0

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
    for (int i = 0; i < g3.getV(); i++) {
        if (postorder2[i] != -1) cout << postorder2[i] << " "; // Ignore unvisited nodes
    }
    cout << endl;
    // Expected output: 3 2

    vector<int> postorder3 = g3.getPostorder(4);
    cout << "Postorder Traversal from Node 4: ";
    for (int i = 0; i < g3.getV(); i++) {
        if (postorder3[i] != -1) cout << postorder3[i] << " "; // Ignore unvisited nodes
    }
    cout << endl;
    // Expected output: 5 4
}

int main() {
    runTests();
    return 0;
}