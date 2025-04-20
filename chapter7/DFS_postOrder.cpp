#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Graph {
private:
    int V; // 图的节点数
    vector<vector<int>> adj; // 邻接表表示图

public:
    // 构造函数
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // 添加边
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // 执行DFS后序遍历
    void DFS_Postorder(int u, vector<bool>& visited, vector<int>& postorder) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                DFS_Postorder(v, visited, postorder);
            }
        }
        postorder.push_back(u); // 后序：先处理子节点，再记录当前节点
    }

    // 获取后序遍历结果
    vector<int> getPostorder(int start) {
        vector<bool> visited(V, false);
        vector<int> postorder;
        DFS_Postorder(start, visited, postorder);
        return postorder;
    }
};

// 测试用例
void runTests() {
    // 测试用例 1: 简单线性图
    cout << "Test Case 1: Linear Graph" << endl;
    Graph g1(5);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);
    g1.addEdge(3, 4);
    vector<int> postorder1 = g1.getPostorder(0);
    cout << "Postorder: ";
    for (int node : postorder1) {
        cout << node << " ";
    }
    cout << endl;

    // 测试用例 2: 有环图
    cout << "\nTest Case 2: Cyclic Graph" << endl;
    Graph g2(4);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(2, 0);
    g2.addEdge(2, 3);
    vector<int> postorder2 = g2.getPostorder(0);
    cout << "Postorder: ";
    for (int node : postorder2) {
        cout << node << " ";
    }
    cout << endl;

    // 测试用例 3: 不连通图
    cout << "\nTest Case 3: Disconnected Graph" << endl;
    Graph g3(6);
    g3.addEdge(0, 1);
    g3.addEdge(2, 3);
    g3.addEdge(4, 5);
    vector<int> postorder3 = g3.getPostorder(0);
    cout << "Postorder (from node 0): ";
    for (int node : postorder3) {
        cout << node << " ";
    }
    cout << endl;

    vector<int> postorder4 = g3.getPostorder(2);
    cout << "Postorder (from node 2): ";
    for (int node : postorder4) {
        cout << node << " ";
    }
    cout << endl;

    vector<int> postorder5 = g3.getPostorder(4);
    cout << "Postorder (from node 4): ";
    for (int node : postorder5) {
        cout << node << " ";
    }
    cout << endl;
}

int main() {
    runTests();
    return 0;
}