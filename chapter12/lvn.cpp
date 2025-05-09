#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

// Forward declarations of types and functions (as they are not fully defined in the pseudocode)
struct Operand {
    std::string kind;
    std::string val;
};

struct MIRInst {
    std::string kind;
    std::string opr;
    Operand opd;
    Operand opd1;
    Operand opd2;
    std::string left;
    std::string lbl;
    std::string trapno;
};

// Placeholder for the Hash function
int Hash(const std::string& opr, const Operand& op1, const Operand& op2) {
    // Simple hash based on string concatenation (for demonstration)
    std::string combined = opr + op1.kind + op1.val + op2.kind + op2.val;
    std::hash<std::string> hasher;
    return hasher(combined);
}

// Placeholder for the Match_Exp function
bool Match_Exp(const MIRInst& inst1, const MIRInst& inst2) {
    // Simple comparison of relevant fields (adjust as needed)
    return inst1.kind == inst2.kind && inst1.opr == inst2.opr &&
           inst1.opd1.kind == inst2.opd1.kind && inst1.opd1.val == inst2.opd1.val &&
           inst1.opd2.kind == inst2.opd2.kind && inst1.opd2.val == inst2.opd2.val;
}

// Placeholder for the Has_Left function
bool Has_Left(const std::string& kind) {
    return kind.find("asn") != std::string::npos; // Assuming "asn" in kind indicates assignment
}

// Placeholder for the Exp_Kind function (returning the kind string directly for simplicity)
std::string Exp_Kind(const std::string& kind) {
    return kind;
}

// Placeholder for new_tmp function
std::string new_tmp() {
    static int temp_count = 0;
    return "temp_" + std::to_string(temp_count++);
}

// Placeholder for insert_after function
void insert_after(int m, int& i, std::vector<int>& ninsts, std::vector<std::vector<MIRInst>>& Block, const MIRInst& new_inst) {
    Block[m].insert(Block[m].begin() + i + 1, new_inst);
    ninsts[m]++;
}

// Placeholder for the Remove function
void Remove(std::vector<std::vector<int>>& f, int maxhash, const std::string& v, int m, int nblocks, std::vector<std::vector<MIRInst>>& Block) {
    for (size_t i = 0; i < f.size(); ++i) {
        if (!f[i].empty()) {
            for (auto it = f[i].begin(); it != f[i].end(); ) {
                if (*it >= 0 && *it < Block[m].size()) {
                    const auto& inst = Block[m][*it];
                    std::string kind = Exp_Kind(inst.kind);
                    if (kind == "binexp") {
                        if (inst.opd1.val == v || inst.opd2.val == v) {
                            it = f[i].erase(it);
                            continue;
                        }
                    } else if (kind == "unexp") {
                        if (inst.opd.val == v) {
                            it = f[i].erase(it);
                            continue;
                        }
                    }
                }
                ++it;
            }
        }
    }
}

int Process_Inst(int m, int& i, int nblocks, std::vector<std::vector<MIRInst>>& Block, const Operand& opnd1, const Operand& opnd2,
                 int& maxhash, std::vector<std::vector<int>>& HashSeq, std::vector<int>& ninsts) {
    MIRInst& inst = Block[m][i];
    int hval = Hash(inst.opr, opnd1, opnd2);
    int retval = 1;
    bool doit = true;

    if (hval >= HashSeq.size()) {
        HashSeq.resize(hval + 1);
    }

    if (hval > maxhash) {
        maxhash = hval;
    } else if (hval < HashSeq.size() && !HashSeq[hval].empty()) {
        for (int index_in_block : HashSeq[hval]) {
            if (index_in_block >= 0 && index_in_block < Block[m].size()) {
                const MIRInst& inst2 = Block[m][index_in_block];
                if (Match_Exp(inst, inst2)) {
                    doit = false;
                    if (Has_Left(inst.kind)) {
                        if (inst.kind.find("asn") != std::string::npos) {
                            inst.kind = "valasn";
                            inst.opd = {"var", inst2.left};
                        } else if (inst.kind == "binif" || inst.kind == "unif") {
                            inst.kind = "valif";
                            inst.opd = {"var", inst2.left};
                        } else if (inst.kind == "bintrap" || inst.kind == "untrap") {
                            inst.kind = "valtrap";
                            inst.opd = {"var", inst2.left};
                        }
                    }
                    break;
                }
            }
        }

        if (Has_Left(inst.kind)) {
            Remove(HashSeq, maxhash, inst.left, m, nblocks, Block);
        }

        if (doit) {
            if (!Has_Left(inst.kind)) {
                std::string tj = new_tmp();
                if (inst.kind == "binif" || inst.kind == "unif") {
                    insert_after(m, i, ninsts, Block, {"valif", "", {"var", tj}, {}, {}, "", Block[m][i].lbl, ""});
                    retval = 2;
                } else if (inst.kind == "bintrap" || inst.kind == "untrap") {
                    insert_after(m, i, ninsts, Block, {"valtrap", "", {"var", tj}, {}, {}, "", "", Block[m][i].trapno});
                    retval = 2;
                }

                if (opnd2.kind.empty()) {
                    Block[m][i] = {"unasgn", inst.opr, opnd1, {}, {}, tj, "", ""};
                } else {
                    Block[m][i] = {"binasn", inst.opr, opnd1, opnd2, {}, tj, "", ""};
                }
            }
            if (hval < HashSeq.size()) {
                HashSeq[hval].push_back(i);
            } else {
                HashSeq.resize(hval + 1);
                HashSeq[hval].push_back(i);
            }
        }
    } else {
        if (hval < HashSeq.size()) {
            HashSeq[hval].push_back(i);
        } else {
            HashSeq.resize(hval + 1);
            HashSeq[hval].push_back(i);
        }
    }

    return retval;
}

void Value_Number(int m, int nblocks, std::vector<int>& ninsts, std::vector<std::vector<MIRInst>>& Block, int& maxhash) {
    std::vector<std::vector<int>> HashSeq(maxhash + 1);
    int i = 0;
    while (i < ninsts[m]) {
        std::string kind = Exp_Kind(Block[m][i].kind);
        int increment = 1;
        if (kind == "binexp") {
            increment = Process_Inst(m, i, nblocks, Block, Block[m][i].opd1, Block[m][i].opd2, maxhash, HashSeq, ninsts);
        } else if (kind == "unexp") {
            increment = Process_Inst(m, i, nblocks, Block, Block[m][i].opd, {}, maxhash, HashSeq, ninsts);
        } else {
            increment = 1;
        }
        i += increment;
    }
}

// Helper function to print the Block
void print_block(const std::vector<MIRInst>& block) {
    for (const auto& inst : block) {
        std::cout << "  Kind: " << inst.kind
                  << ", Opr: " << inst.opr
                  << ", Opd1: {" << inst.opd1.kind << ":" << inst.opd1.val << "}"
                  << ", Opd2: {" << inst.opd2.kind << ":" << inst.opd2.val << "}"
                  << ", Opd: {" << inst.opd.kind << ":" << inst.opd.val << "}"
                  << ", Left: " << inst.left
                  << ", Lbl: " << inst.lbl
                  << ", Trapno: " << inst.trapno
                  << std::endl;
    }
}

int main() {
    // Test Case
    std::vector<std::vector<MIRInst>> blocks = {
        {
            {"binasn", "+", {"var", "a"}, {"const", "10"}, {}, "x", "", ""}, // a + 10 -> x
            {"binasn", "+", {"var", "b"}, {"const", "10"}, {}, "y", "", ""}, // b + 10 -> y
            {"binasn", "*", {"var", "x"}, {"var", "y"}, {}, "z", "", ""}, // x * y -> z
            {"unasn", "-", {"var", "x"}, {}, {}, "w", "", ""}, // -x -> w
            {"binif", "==", {"var", "z"}, {"const", "100"}, {}, "", "label1", ""}, // label: z == 100
            {"bintrap", "<", {"var", "w"}, {"const", "0"}, {}, "", "", "trap1"} // w < 0 trap1
        }
    };

    int nblocks = blocks.size();
    std::vector<int> ninsts = {static_cast<int>(blocks[0].size())};
    int maxhash = 0;

    std::cout << "Original Block:\n";
    print_block(blocks[0]);
    std::cout << std::endl;

    Value_Number(0, nblocks, ninsts, blocks, maxhash);

    std::cout << "Optimized Block:\n";
    print_block(blocks[0]);
    std::cout << std::endl;

    return 0;
}