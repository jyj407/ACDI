#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

using namespace std;

class ImmediateDominators {
public:
    // 输入：节点集合 N, 支配节点映射 Domin, 起始节点 r
    unordered_map<int, int> computeIdom(const vector<int>& nodes, const unordered_map<int, set<int>>& domin, int root) {
        unordered_map<int, int> idom; // 存储直接支配节点
        unordered_map<int, set<int>> tmp; // 临时集合 Tmp(n)

        // 初始化 Tmp(n)
        for (int n : nodes) {
            if (n == root) continue; // 起始节点没有直接支配节点
            tmp[n] = domin.at(n);    // 获取支配集合
            tmp[n].erase(n);         // 移除自身
        }

        // 逐步计算每个节点的直接支配节点
        for (int n : nodes) {
            if (n == root) continue;

            set<int>& tmpSet = tmp[n];
            set<int> toErase; // 用于存储需要删除的节点

            for (int s : tmpSet) {
                for (int t : tmpSet) {
                    if (s != t && domin.at(s).count(t)) {
                        toErase.insert(t); // 收集要删除的节点
                    }
                }
            }

            // 统一删除多余节点
            for (int t : toErase) {
                tmpSet.erase(t);
            }

            // Tmp(n) 应只剩一个元素，即直接支配节点
            if (!tmpSet.empty()) {
                idom[n] = *tmpSet.begin();
            }
        }

        return idom;
    }
};

// 测试用例
int main() {
    ImmediateDominators idomCalculator;

    // 示例输入：节点集合
    // vector<int> nodes = {0, 1, 2, 3, 4, 5, 6, 7};
    // // 示例输入：支配节点映射
    // unordered_map<int, set<int>> domin = {
    //     {0, {0}},
    //     {1, {0, 1}},
    //     {2, {0, 1, 2}},
    //     {3, {0, 1, 3}},
    //     {4, {0, 1, 3, 4}},
    //     {5, {0, 1, 3, 4, 5}},
    //     {6, {0, 1, 3, 4, 6}},
    //     {7, {0, 1, 7}}
    // };
    // int root = 0;

    // Test 2, from 7.4 of ACDI
    vector<int> nodes = {0, 1, 2, 3, 4, 5, 6, 7};
    unordered_map<int, string> IdxToName = {
        {0, "entry"},
        {1, "B1"},
        {2, "B2"},
        {3, "B3"},
        {4, "B4"},
        {5, "B5"},
        {6, "B6"},
        {7, "exit"}
    };
    // 示例输入：支配节点映射
    unordered_map<int, set<int>> domin = {
        {0, {0}},
        {1, {0, 1}},
        {2, {0, 1, 2}},
        {3, {0, 1, 3}},
        {4, {0, 1, 3, 4}},
        {5, {0, 1, 3, 4, 5}},
        {6, {0, 1, 3, 4, 6}},
        {7, {0, 1, 7}}
    };
    int root = 0;
    // 计算直接支配节点
    unordered_map<int, int> idom = idomCalculator.computeIdom(nodes, domin, root);

    // 输出结果
    for (const auto& pair : idom) {
        // cout << "Node " << pair.first << " is immediately dominated by " << pair.second << endl;
        cout << "Node " << IdxToName[pair.first] << " is immediately dominated by " << IdxToName[pair.second] << endl;
    }

    return 0;
}