#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

class Graph {
private:
    int V;                           // 节点数量
    vector<vector<int>> adj;         // 邻接表表示图结构
    vector<vector<int>> reverseAdj;  // 反向邻接表，用于计算前驱

public:
    // 构造函数
    Graph(int V) {
        this->V = V;
        adj.resize(V);
        reverseAdj.resize(V);
    }

    // 添加边
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        reverseAdj[v].push_back(u);
    }

    // 获取后继节点
    vector<int> getSuccessors(int node) {
        return adj[node];
    }

    // 获取前驱节点
    vector<int> getPredecessors(int node) {
        return reverseAdj[node];
    }

    // 判断是否是 Block
    bool isBlock(int start, unordered_set<int>& nset) {
        int current = start;

        // Forward traversal: 按后继节点向前扩展 Block
        while (true) {
            vector<int> successors = getSuccessors(current);
            vector<int> predecessors = getPredecessors(current);

            if (successors.size() == 1 && getPredecessors(successors[0]).size() == 1) {
                nset.insert(successors[0]);
                current = successors[0];
            } else {
                break;
            }
        }

        // Backward traversal: 按前驱节点向后扩展 Block
        current = start;
        while (true) {
            vector<int> successors = getSuccessors(current);
            vector<int> predecessors = getPredecessors(current);

            if (predecessors.size() == 1 && getSuccessors(predecessors[0]).size() == 1) {
                nset.insert(predecessors[0]);
                current = predecessors[0];
            } else {
                break;
            }
        }

        // Block 的节点数量需 >= 2
        return nset.size() >= 2;
    }

    // 判断是否是 IfThenElse
    bool isIfThenElse(int node, unordered_set<int>& nset) {
        vector<int> successors = getSuccessors(node);

        // 必须有两个后继节点
        if (successors.size() != 2) return false;

        int m = successors[0];
        int n = successors[1];

        // 后继的后继必须一致，且每个后继只有一个前驱
        if (getSuccessors(m) != getSuccessors(n) || getPredecessors(m).size() != 1 || getPredecessors(n).size() != 1) {
            return false;
        }

        // 确认两个后继节点分别加入 nset
        nset.insert(node);
        nset.insert(m);
        nset.insert(n);
        return true;
    }

    // 识别区域类型
    string identifyRegionType(int node) {
        unordered_set<int> nset;

        if (isBlock(node, nset)) {
            return "Block";
        } else if (isIfThenElse(node, nset)) {
            return "IfThenElse";
        }

        return "Unknown";
    }
};

// 测试用例
void runTests() {
    cout << "Test Case 1: Block" << endl;
    Graph g1(5);
    g1.addEdge(0, 1);
    g1.addEdge(1, 2);
    g1.addEdge(2, 3);
    g1.addEdge(3, 4);
    cout << "Region Type starting at Node 0: " << g1.identifyRegionType(0) << endl;
    // Expected output: Block

    cout << "\nTest Case 2: IfThenElse" << endl;
    Graph g2(5);
    g2.addEdge(0, 1);
    g2.addEdge(0, 2);
    g2.addEdge(1, 3);
    g2.addEdge(2, 3);
    cout << "Region Type starting at Node 0: " << g2.identifyRegionType(0) << endl;
    // Expected output: IfThenElse

    cout << "\nTest Case 3: Unknown Structure" << endl;
    Graph g3(4);
    g3.addEdge(0, 1);
    g3.addEdge(0, 2);
    cout << "Region Type starting at Node 0: " << g3.identifyRegionType(0) << endl;
    // Expected output: Unknown
}

int main() {
    runTests();
    return 0;
}