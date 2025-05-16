#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <tuple>

using namespace std;

typedef tuple<string, string, string, string> Instruction;  // (operator, operand1, operand2, result)

class GlobalCSE {
private:
    map<tuple<string, string, string>, string> globalExprs; // Track global expressions across blocks

public:
    vector<vector<Instruction>> optimize(vector<vector<Instruction>>& blocks) {
        vector<vector<Instruction>> optimizedBlocks;

        for (auto& block : blocks) {
            vector<Instruction> optimizedBlock;
            for (auto& instr : block) {
                string op, arg1, arg2, result;
                tie(op, arg1, arg2, result) = instr;

                // Normalize operands for commutative operators
                tuple<string, string, string> expr = (arg1 < arg2) ? make_tuple(op, arg1, arg2) : make_tuple(op, arg2, arg1);

                if (globalExprs.find(expr) != globalExprs.end()) {
                    optimizedBlock.emplace_back("=", globalExprs[expr], "", result); // Replace redundant computation
                } else {
                    globalExprs[expr] = result; // Store computed expression globally
                    optimizedBlock.push_back(instr); // Keep original computation
                }
            }
            optimizedBlocks.push_back(optimizedBlock);
        }

        return optimizedBlocks;
    }
};

// Function to print optimized code blocks
void printOptimizedBlocks(const vector<vector<Instruction>>& blocks) {
    for (size_t i = 0; i < blocks.size(); ++i) {
        cout << "Block " << i + 1 << ":\n";
        for (const auto& instr : blocks[i]) {
            string op, arg1, arg2, result;
            tie(op, arg1, arg2, result) = instr;
            if (op == "=") {
                cout << result << " = " << arg1 << "\n";
            } else {
                cout << result << " = " << arg1 << " " << op << " " << arg2 << "\n";
            }
        }
        cout << "-----------------------\n";
    }
}

// ```cpp
// Test cases
void runTests() {
    vector<vector<Instruction>> blocks = {
        { {"*", "a", "b", "t1"}, {"+", "a", "b", "t2"} },
        { {"*", "a", "b", "t3"}, {"+", "a", "b", "t4"} }, // Redundant computations
    };

    GlobalCSE optimizer;
    vector<vector<Instruction>> optimizedBlocks = optimizer.optimize(blocks);

    cout << "Optimized Code Blocks:\n";
    printOptimizedBlocks(optimizedBlocks);
}

int main() {
    runTests();
    return 0;
}