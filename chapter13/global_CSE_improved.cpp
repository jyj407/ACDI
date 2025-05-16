// With killed expression removal logic
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <tuple>

using namespace std;

typedef tuple<string, string, string, string> Instruction;  // (operator, operand1, operand2, result)

class GlobalCSE {
private:
    map<tuple<string, string, string>, string> globalExprs;  // Tracks global common subexpressions
    set<string> redefinedVars;  // Tracks redefined variables to remove invalid expressions

public:
    vector<vector<Instruction>> optimize(vector<vector<Instruction>>& blocks) {
        vector<vector<Instruction>> optimizedBlocks;

        for (auto& block : blocks) {
            vector<Instruction> optimizedBlock;

            for (auto& instr : block) {
                string op, arg1, arg2, result;
                tie(op, arg1, arg2, result) = instr;

                // If a variable is being redefined, remove related expressions
                if (op == "=") {
                    redefinedVars.insert(result);

                    // Remove expressions that use the redefined variable
                    for (auto it = globalExprs.begin(); it != globalExprs.end();) {
                        if (redefinedVars.count(get<1>(it->first)) || redefinedVars.count(get<2>(it->first))) {
                            it = globalExprs.erase(it);  // Remove invalidated expression
                        } else {
                            ++it;
                        }
                    }
                }

                // Normalize operands for commutative operators
                tuple<string, string, string> expr = (arg1 < arg2) ? make_tuple(op, arg1, arg2) : make_tuple(op, arg2, arg1);

                // If this expression exists globally, replace with the stored result
                if (globalExprs.find(expr) != globalExprs.end() && !redefinedVars.count(result)) {
                    optimizedBlock.emplace_back("=", globalExprs[expr], "", result);
                } else {
                    globalExprs[expr] = result;  // Store computed expression globally
                    optimizedBlock.push_back(instr);  // Keep original computation
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

// Test cases with variable redefinitions
void runTests() {
    vector<vector<Instruction>> blocks = {
        { {"*", "a", "b", "t1"}, {"+", "a", "b", "t2"}, {"=", "c", "", "a"} },  // `a` is redefined, invalidating earlier expressions
        { {"*", "a", "b", "t3"}, {"+", "a", "b", "t4"} },  // Shouldn't reuse previous expressions from Block 1
    };

    GlobalCSE optimizer;
    vector<vector<Instruction>> optimizedBlocks = optimizer.optimize(blocks);

    cout << "Optimized Code Blocks:\n";
    printOptimizedBlocks(optimizedBlocks);
}

void runTests2() {
    vector<vector<Instruction>> blocks = {
        { {"*", "a", "b", "t1"}, {"+", "b", "d", "t2"}, {"=", "c", "", "a"} },  // `a` is redefined, invalidating earlier expressions
        { {"*", "a", "b", "t3"}, {"+", "b", "d", "t4"} },  // only b + d can be reused, a + b cannot, since 'a' is invalidated
    };

    GlobalCSE optimizer;
    vector<vector<Instruction>> optimizedBlocks = optimizer.optimize(blocks);

    cout << "Optimized Code Blocks:\n";
    printOptimizedBlocks(optimizedBlocks);
}

int main() {
    runTests();
    runTests2();
    return 0;
}