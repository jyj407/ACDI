#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
using namespace std;

class Graph {
private:
    int V; // Number of vertices
    vector<vector<int>> adj;

    void strongComponents(int u, vector<int>& dfn, vector<int>& lowlink, stack<int>& st, vector<bool>& onStack, int& index, vector<vector<int>>& allSCC);

public:
    Graph(int V);
    void addEdge(int v, int w);
    void findSCCs();
};

Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
}

void Graph::strongComponents(int u, vector<int>& dfn, vector<int>& lowlink, stack<int>& st, vector<bool>& onStack, int& index, vector<vector<int>>& allSCC) {
    dfn[u] = lowlink[u] = index++;
    st.push(u);
    onStack[u] = true;

    for (int v : adj[u]) {
        if (dfn[v] == -1) {
            strongComponents(v, dfn, lowlink, st, onStack, index, allSCC);
            lowlink[u] = min(lowlink[u], lowlink[v]);
        } else if (onStack[v]) {
            lowlink[u] = min(lowlink[u], dfn[v]);
        }
    }

    if (lowlink[u] == dfn[u]) {
        vector<int> scc;
        int v;
        do {
            v = st.top();
            st.pop();
            onStack[v] = false;
            scc.push_back(v);
        } while (v != u);
        allSCC.push_back(scc);
    }
}

void Graph::findSCCs() {
    vector<int> dfn(V, -1);
    vector<int> lowlink(V, -1);
    stack<int> st;
    vector<bool> onStack(V, false);
    int index = 0;
    vector<vector<int>> allSCC;

    for (int i = 0; i < V; i++) {
        if (dfn[i] == -1) {
            strongComponents(i, dfn, lowlink, st, onStack, index, allSCC);
        }
    }

    // Print all SCCs
    for (const auto& scc : allSCC) {
        cout << "SCC: ";
        for (int v : scc) {
            cout << v << " ";
        }
        cout << endl;
    }
}

// Test cases and main function
void runTests() {
    cout << "Test Case 1: Single SCC (Single Node)" << endl;
    Graph g1(1);
    g1.findSCCs(); // Expected output: SCC: 0

    cout << "\nTest Case 2: Two Nodes, One Edge" << endl;
    Graph g2(2);
    g2.addEdge(0, 1);
    g2.findSCCs(); // Expected output: SCC: 0, SCC: 1

    cout << "\nTest Case 3: Cycle Graph (Single SCC)" << endl;
    Graph g3(3);
    g3.addEdge(0, 1);
    g3.addEdge(1, 2);
    g3.addEdge(2, 0);
    g3.findSCCs(); // Expected output: SCC: 0 1 2

    cout << "\nTest Case 4: Multiple SCCs" << endl;
    Graph g4(5);
    g4.addEdge(0, 1);
    g4.addEdge(1, 2);
    g4.addEdge(2, 0);
    g4.addEdge(1, 3);
    g4.addEdge(3, 4);
    g4.addEdge(4, 3);
    g4.findSCCs(); // Expected output: SCC: 0 1 2, SCC: 3 4

    cout << "\nTest Case 5: Disconnected Graph" << endl;
    Graph g5(4);
    g5.addEdge(0, 1);
    g5.addEdge(2, 3);
    g5.findSCCs(); // Expected output: SCC: 0, SCC: 1, SCC: 2, SCC: 3
}

int main() {
    runTests();
    return 0;
}