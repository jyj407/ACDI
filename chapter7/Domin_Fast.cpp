#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>

using namespace std;

class DominatorTree {
private:
    vector<int> label, ancestor, semi, idom, vertex, parent;
    vector<vector<int>> bucket, graph, reverseGraph;
    int n;

    void dfs(int v, int& index) {
        semi[v] = ++index;
        vertex[index] = v;
        label[v] = v;
        ancestor[v] = -1; // 初始化祖先为 -1
        for (int u : graph[v]) {
            if (semi[u] == 0) { // 如果未访问过
                parent[u] = v;
                dfs(u, index);
            }
            reverseGraph[u].push_back(v); // 构建反向图
        }
    }

    int eval(int v) {
        if (ancestor[v] == -1) return label[v]; // 没有祖先时直接返回标签
        compress(v); // 路径压缩
        return label[v];
    }

    void compress(int v) {
        stack<int> s;
        int a = v;
        while (ancestor[ancestor[a]] != -1) { // 找到根
            s.push(a);
            a = ancestor[a];
        }
        while (!s.empty()) { // 压缩路径
            int x = s.top();
            s.pop();
            if (semi[label[x]] > semi[label[ancestor[x]]]) {
                label[x] = label[ancestor[x]];
            }
            ancestor[x] = ancestor[a];
        }
    }

    void link(int v, int u) {
        ancestor[u] = v; // 将 u 连接到 v
    }

public:
    DominatorTree(int size) : n(size), graph(size), reverseGraph(size), bucket(size),
                              semi(size + 1, 0), idom(size + 1, -1), vertex(size + 1, -1),
                              label(size + 1, -1), ancestor(size + 1, -1), parent(size + 1, -1) {}

    void addEdge(int u, int v) {
        if (u >= n || v >= n || u < 0 || v < 0) {
            throw runtime_error("Invalid edge or node index!");
        }
        graph[u].push_back(v); // 添加边到图
    }

    vector<int> compute(int root) {
        int index = 0;
        dfs(root, index);

        for (int i = n; i > 0; --i) { // 从最后一个节点开始计算
            int w = vertex[i];
            for (int v : reverseGraph[w]) {
                int u = eval(v);
                semi[w] = min(semi[w], semi[u]);
            }
            bucket[vertex[semi[w]]].push_back(w);
            if (parent[w] != -1) { // 检查 parent[w] 是否有效
                link(parent[w], w);
        
                for (int v : bucket[parent[w]]) {
                    int u = eval(v);
                    idom[v] = (semi[u] < semi[v] ? u : parent[w]);
                }
                bucket[parent[w]].clear(); // 清空桶以避免重复处理
            } else {
                cerr << "Warning: parent[" << w << "] is -1. Skipping bucket processing for node " << w << endl;
            }
        }

        for (int i = 1; i < n; ++i) { // 修正直接支配节点
            int w = vertex[i];
            if ((idom[w] != vertex[semi[w]]) && (idom[w] != -1)) {
                idom[w] = idom[idom[w]];
            }
        }
        idom[root] = -1; // 根节点没有直接支配节点
        return idom;
    }
};

// 测试用例
int main() {
    int n = 7; // 图的节点数
    DominatorTree dt(n);

    // 示例图：添加边
    dt.addEdge(0, 1);
    dt.addEdge(0, 2);
    dt.addEdge(1, 3);
    dt.addEdge(1, 4);
    dt.addEdge(2, 5);
    dt.addEdge(2, 6);

    // 计算直接支配节点
    vector<int> idom = dt.compute(0);

    // 输出结果
    cout << "直接支配节点 (Immediate Dominators):" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "节点 " << i << " 的直接支配节点是 " << (idom[i] == -1 ? "null" : to_string(idom[i])) << endl;
    }

    return 0;
}