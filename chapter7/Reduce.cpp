#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
using namespace std;

// Hashing pairs for unordered_set and unordered_map
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

class Graph {
private:
    int nodeCounter; // Counter to create unique abstract nodes
    unordered_set<int> N; // Set of nodes
    vector<pair<int, int>> E; // Edges as pairs of nodes
    unordered_map<int, unordered_set<int>> Pred; // Predecessors of nodes
    unordered_map<int, unordered_set<int>> Succ; // Successors of nodes
    unordered_map<int, string> StructType; // Structure type for abstract nodes
    unordered_map<int, vector<int>> StructNodes; // Original nodes compacted into an abstract node
    unordered_set<int> CTNodes; // Control tree nodes
    vector<pair<int, int>> CTEdges; // Control tree edges

public:
    Graph(int V) {
        nodeCounter = V; // Abstract nodes start after existing nodes
        for (int i = 0; i < V; i++) {
            N.insert(i);
        }
    }

    void addEdge(int u, int v) {
        E.emplace_back(u, v);
        Succ[u].insert(v);
        Pred[v].insert(u);
    }

    void Replace(int node, const vector<int>& NodeSet) {
        // Create new abstract node and compact NodeSet
        int ctnode = nodeCounter++;
        N.insert(ctnode);

        unordered_set<int> nodeSetLookup(NodeSet.begin(), NodeSet.end());
        vector<pair<int, int>> newEdges; // Temporary container for new edges
        unordered_set<pair<int, int>, hash_pair> edgesToRemove; // Mark edges to remove

        // Iterate through all edges and process them
        for (const auto& edge : E) {
            int u = edge.first, v = edge.second;

            // If the edge involves the NodeSet, process it
            if (nodeSetLookup.count(u) || nodeSetLookup.count(v)) {
                edgesToRemove.insert(edge); // Mark this edge for removal

                // Redirect outgoing edges to the abstract node
                if (nodeSetLookup.count(u) && !nodeSetLookup.count(v)) {
                    newEdges.emplace_back(ctnode, v);
                    Pred[v].insert(ctnode);
                    Succ[ctnode].insert(v);
                }

                // Redirect incoming edges to the abstract node
                if (nodeSetLookup.count(v) && !nodeSetLookup.count(u)) {
                    newEdges.emplace_back(u, ctnode);
                    Succ[u].insert(ctnode);
                    Pred[ctnode].insert(u);
                }
            }
        }

        // Remove edges that involve NodeSet
        auto it = E.begin();
        while (it != E.end()) {
            if (edgesToRemove.count(*it)) {
                // Remove edge from the graph
                Succ[it->first].erase(it->second);
                Pred[it->second].erase(it->first);
                it = E.erase(it); // Safely erase and move the iterator
            } else {
                ++it;
            }
        }

        // Add new edges to the graph
        E.insert(E.end(), newEdges.begin(), newEdges.end());

        // Remove nodes in NodeSet from the graph
        for (int n : NodeSet) {
            N.erase(n);
            Succ.erase(n);
            Pred.erase(n);
        }

        // Update control tree
        CTNodes.insert(ctnode);
        for (int n : NodeSet) {
            CTEdges.emplace_back(ctnode, n);
        }
    }

    void Reduce(vector<int>& NodeSet, const string& rtype) {
        int node = nodeCounter++; // Create new abstract node
        Replace(node, NodeSet);

        // Assign structural information
        StructType[node] = rtype;
        StructNodes[node] = NodeSet;

        // Termination check: Ensure NodeSet is fully removed
        for (int n : NodeSet) {
            if (N.count(n)) {
                cerr << "ERROR: NodeSet was not fully removed from graph!" << endl;
                exit(1); // Terminate to prevent infinite loop
            }
        }

        cout << "Reduction completed for NodeSet." << endl;
    }

    void printGraph() {
        cout << "Nodes:" << endl;
        for (int n : N) {
            cout << n << " ";
        }
        cout << endl;

        cout << "Edges:" << endl;
        for (auto& edge : E) {
            cout << edge.first << " -> " << edge.second << endl;
        }
    }

    void printControlTree() {
        cout << "\nControl Tree Nodes:" << endl;
        for (int node : CTNodes) {
            cout << node << " ";
        }
        cout << endl;

        cout << "Control Tree Edges:" << endl;
        for (auto& edge : CTEdges) {
            cout << edge.first << " -> " << edge.second << endl;
        }
    }
};

// Test Cases
void runTests() {
    cout << "Test Case 1: Reduce a loop region" << endl;
    Graph g1(5);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);
    g1.addEdge(3, 0); // Back edge
    g1.addEdge(3, 4); // Exit edge

    cout << "Original Graph:" << endl;
    g1.printGraph();

    vector<int> nodeSet = {0, 1, 2, 3};
    g1.Reduce(nodeSet, "NaturalLoop");

    cout << "\nReduced Graph:" << endl;
    g1.printGraph();
    g1.printControlTree();

    cout << "\nTest Case 2: Reduce a conditional region" << endl;
    Graph g2(4);
    g2.addEdge(0, 1);
    g2.addEdge(0, 2);
    g2.addEdge(1, 3);
    g2.addEdge(2, 3); // Converging edges

    cout << "\nOriginal Graph:" << endl;
    g2.printGraph();

    vector<int> condSet = {1, 2};
    g2.Reduce(condSet, "IfThenElse");

    cout << "\nReduced Graph:" << endl;
    g2.printGraph();
    g2.printControlTree();
}

int main() {
    runTests();
    return 0;
}