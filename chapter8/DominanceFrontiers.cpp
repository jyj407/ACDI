#include <iostream>
#include <vector>
#include <set>
#include <map>

// Function to compute dominance frontiers
void computeDominanceFrontier(int X, const std::map<int, std::vector<int>>& succ,
                              const std::map<int, int>& idom, const std::map<int, std::vector<int>>& children,
                              std::map<int, std::set<int>>& DF) {
    DF[X].clear();
    for (int Y : succ.at(X)) {
        if (idom.at(Y) != X) {
            DF[X].insert(Y);
        }
    }
    for (int Z : children.at(X)) {
        for (int Y : DF[Z]) {
            if (idom.at(Y) != X) {
                DF[X].insert(Y);
            }
        }
    }
}

// Test function to validate the dominance frontier computation
void testDominanceFrontier() {
    // Example graph
    std::map<int, std::vector<int>> succ = {
        {1, {2, 3}},
        {2, {4}},
        {3, {4}},
        {4, {5}},
        {5, {}}
    };

    std::map<int, int> idom = {
        {1, 1},
        {2, 1},
        {3, 1},
        {4, 1},
        {5, 4}
    };

    std::map<int, std::vector<int>> children = {
        {1, {2, 3, 4}},
        {2, {}},
        {3, {}},
        {4, {5}},
        {5, {}}
    };

    std::map<int, std::set<int>> DF;

    // Compute dominance frontier for each node
    for (int X = 5; X >= 1; --X) {
        computeDominanceFrontier(X, succ, idom, children, DF);
    }

    // Print the dominance frontiers
    for (const auto& pair : DF) {
        std::cout << "DF(" << pair.first << ") = {";
        for (auto it = pair.second.begin(); it != pair.second.end(); ++it) {
            if (it != pair.second.begin()) {
                std::cout << ", ";
            }
            std::cout << *it;
        }
        std::cout << "}" << std::endl;
    }
}

int main() {
    testDominanceFrontier();
    return 0;
}