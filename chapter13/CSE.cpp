#include <iostream>
#include <vector>
#include <map>
#include <tuple>

using namespace std;

// Define an instruction as a tuple (operator, operand1, operand2, result)
typedef tuple<string, string, string, string> Instruction;

class LocalCSE {
private:
    map<tuple<string, string, string>, string> aeb; // Available expressions in block
    
public:
    vector<Instruction> optimize(const vector<Instruction>& instructions) {
        vector<Instruction> optimized_code;
        
        for (const auto& instr : instructions) {
            auto [op, arg1, arg2, result] = instr; // use structure binding to unpack instr
            
            // Handle commutative operations by sorting operands
            tuple<string, string, string> expr = (arg1 < arg2) ? make_tuple(op, arg1, arg2) : make_tuple(op, arg2, arg1);
            
            if (aeb.find(expr) != aeb.end()) {
                // Replace redundant computation with stored result
                optimized_code.emplace_back("=", aeb[expr], "", result);
            } else {
                // Store computed expression
                aeb[expr] = result;
                optimized_code.push_back(instr); // Keep original instruction
            }
        }
        
        return optimized_code;
    }
};

int main() {
    vector<Instruction> instructions = {
        {"*", "a", "b", "t1"},
        {"+", "a", "b", "t2"},
        {"*", "a", "b", "t3"},  // Redundant, should use t1
        {"+", "a", "b", "t4"}   // Redundant, should use t2
    };

    LocalCSE optimizer;
    vector<Instruction> optimized_code = optimizer.optimize(instructions);

    cout << "Optimized Instructions:\n";
    for (const auto& instr : optimized_code) {
        auto [op, arg1, arg2, result] = instr;
        cout << result << " = " << arg1 << " " << op << " " << arg2 << endl;
    }

    return 0;
}