#include <iostream>
#include <vector>
#include <unordered_set>
#include <stack>
#include <unordered_map>
using namespace std;

class NaturalLoop {
public:
    // 输入：反向边 m->n 和前驱关系图
    unordered_set<int> computeNatLoop(int m, int n, unordered_map<int, vector<int>>& Pred) {
        unordered_set<int> Loop; // 用于存储循环中的节点
        stack<int> Stack;       // 用于DFS的栈

        // 初始化 Loop 包含 m 和 n
        Loop.insert(m);
        Loop.insert(n);

        // 如果 m ≠ n，则将 m 压入栈
        if (m != n) {
            Stack.push(m);
        }

        // 开始处理栈中的节点
        while (!Stack.empty()) {
            int p = Stack.top();
            Stack.pop();

            // 遍历 p 的所有前驱节点
            for (int q : Pred[p]) {
                if (!Loop.count(q)) { // 如果 q 不在 Loop 中
                    Loop.insert(q);
                    Stack.push(q); // 将 q 压入栈以便后续处理
                }
            }
        }

        return Loop;
    }
};

int main() {
    // 定义前驱关系图
    unordered_map<int, vector<int>> Pred;
    Pred[1] = {0};
    Pred[2] = {1};
    Pred[3] = {2};
    Pred[4] = {2, 3};
    Pred[5] = {4};
    Pred[6] = {5};
    Pred[7] = {6};

    NaturalLoop nl;
    int m = 5; // 反向边的起点
    int n = 2; // 反向边的终点

    // 计算自然循环
    unordered_set<int> loop = nl.computeNatLoop(m, n, Pred);

    // 打印结果
    cout << "自然循环中的节点: ";
    for (int node : loop) {
        cout << node << " ";
    }
    cout << endl;

    return 0;
}