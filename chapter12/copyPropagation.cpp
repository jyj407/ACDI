#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <variant>

// Forward declarations
struct Operand;
struct MIRInst;
std::string copyValue(const Operand& opnd, const std::set<std::pair<std::string, std::string>>& acp);
void removeACP(std::set<std::pair<std::string, std::string>>& acp, const std::string& v);
std::string expKind(const std::string& kind); // Placeholder for expression kind

// Represents a variable or a constant
struct Value {
    std::string kind; // "var" or "const"
    std::string val;
};

// Represents an operand
struct Operand {
    std::string kind; // "var", "const", etc.
    Value val;
};

// Represents a Machine Intermediate Representation Instruction
struct MIRInst {
    std::string kind;
    std::string opr;
    Operand opd1;
    Operand opd2;
    Operand opd;
    std::vector<Operand> argsij;
    std::string left; // Left-hand side of assignment
};

std::string expKind(const std::string& kind) {
    return kind; // Replace with actual logic to determine expression kind
}

std::string copyValue(const Operand& opnd, const std::set<std::pair<std::string, std::string>>& acp) {
    if (opnd.kind == "var") {
        for (const auto& pair : acp) {
            if (opnd.val.val == pair.second) {
                return pair.first;
            }
        }
    }
    return opnd.val.val;
}

void removeACP(std::set<std::pair<std::string, std::string>>& acp, const std::string& v) {
    std::set<std::pair<std::string, std::string>> newACP;
    for (const auto& pair : acp) {
        if (pair.first != v && pair.second != v) {
            newACP.insert(pair);
        }
    }
    acp = newACP;
}

void localCopyProp(int m, int n, std::vector<std::vector<MIRInst>>& block) {
    std::set<std::pair<std::string, std::string>> ACP;
    for (int i = 0; i < n; ++i) {
        MIRInst& inst = block[m][i];
        std::string kind = expKind(inst.kind);

        if (kind == "binexp") {
            inst.opd1.val.val = copyValue(inst.opd1, ACP);
            inst.opd2.val.val = copyValue(inst.opd2, ACP);
        } else if (kind == "unexp") {
            inst.opd.val.val = copyValue(inst.opd, ACP);
        } else if (kind == "lexexp") {
            for (auto& arg : inst.argsij) {
                arg.val.val = copyValue(arg, ACP);
            }
        }

        // Check if the instruction is an assignment (simplified check)
        if (!inst.left.empty()) {
            removeACP(ACP, inst.left);
            // New assignment
            if (inst.kind == "valasn" && inst.opd.kind == "var") {
                ACP.insert({inst.left, inst.opd.val.val});
            }
        }
    }
}

// Helper function to print a MIRInst for testing
void printMIRInst(const MIRInst& inst) {
    std::cout << "Kind: " << inst.kind << ", Left: " << inst.left;
    if (inst.kind == "binexp") {
        std::cout << ", Opd1: {" << inst.opd1.kind << ":" << inst.opd1.val.val << "}, Opd2: {" << inst.opd2.kind << ":" << inst.opd2.val.val << "}";
    } else if (inst.kind == "unexp") {
        std::cout << ", Opd: {" << inst.opd.kind << ":" << inst.opd.val.val << "}";
    } else if (inst.kind == "valasn") {
        std::cout << ", Opd: {" << inst.opd.kind << ":" << inst.opd.val.val << "}";
    }
    std::cout << std::endl;
}

int main1() {
    // Example Usage:
    std::vector<std::vector<MIRInst>> blocks = {
        {
            {"valasn", "", {}, {}, {"var", {"var", "b"}}, {}, "a"}, // a = b
            {"binexp", "+", {"var", {"var", "a"}}, {"var", {"var", "c"}}, {}, {}, "d"},  // d = a + c
            {"valasn", "", {}, {}, {"var", {"var", "d"}}, {}, "e"}, // e = d
            {"binexp", "*", {"var", {"var", "e"}}, {"const", {"const", "2"}}, {}, {}, "f"} // f = e + 2
        }
    };

    int blockIndex = 0;
    int numInstructions = blocks[blockIndex].size();

    std::cout << "Original Block:\n";
    for (const auto& inst : blocks[blockIndex]) {
        printMIRInst(inst);
    }
    std::cout << "\n";

    localCopyProp(blockIndex, numInstructions, blocks);

    std::cout << "After Local Copy Propagation:\n";
    for (const auto& inst : blocks[blockIndex]) {
        printMIRInst(inst);
    }

    return 0;
}

int main() {
    // Example Usage (Corrected to show propagation):
    std::vector<std::vector<MIRInst>> blocks = {
        {
            {"valasn", "", {}, {}, {"var", {"var", "b"}}, {}, "a"}, // a = b
            {"binexp", "+", {"var", {"var", "a"}}, {"var", {"var", "c"}}, {}, {}, "d"}, // d = a + c
            {"valasn", "", {}, {}, {"var", {"var", "d"}}, {}, "e"}, // e = d
            {"binexp", "*", {"var", {"var", "e"}}, {"const", {"const", "2"}}, {}, {}, "f"}  // f = e * 2
        }
    };

    int blockIndex = 0;
    int numInstructions = blocks[blockIndex].size();

    std::cout << "Original Block:\n";
    for (const auto& inst : blocks[blockIndex]) {
        printMIRInst(inst);
    }
    std::cout << "\n";

    localCopyProp(blockIndex, numInstructions, blocks);

    std::cout << "After Local Copy Propagation:\n";
    for (const auto& inst : blocks[blockIndex]) {
        printMIRInst(inst);
    }

    return 0;
}