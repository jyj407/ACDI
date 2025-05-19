#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

// Forward declarations (replace with actual definitions if available)
struct MIRInst {
    std::string kind;
    std::string opd1;
    std::string opd2;
    std::string opd;
    std::string left;
};

bool Loop_Const(const std::string& operand, int block_index, int nblocks, const std::vector<int>& ninsts, const std::vector<std::vector<MIRInst>>& block) {
    // Placeholder: Implement logic to check if an operand is loop-constant
    // This would involve analyzing definitions and uses within the loop.
    // For this example, we'll just return false.
    return false;
}

bool Reach_Defs_Out(const std::string& variable, int block_index, int inst_index, int nblocks, const std::vector<int>& ninsts, const std::vector<std::vector<MIRInst>>& block) {
    // Placeholder: Implement logic to check if a definition of a variable is reaching the LICM variable inside loop from outside the loop
    return false;
}

bool Reach_Defs_In_From_Other_Invariant(const std::string& variable, int block_index, int inst_index, int nblocks, const std::vector<int>& ninsts, const std::vector<std::vector<MIRInst>>& block) {
    // Placeholder: Implement logic to check if a definition of a variable is reaching the LICM variable inside loop from inside the loop ealier position and that def has been marked as LICM
    return false;
}

// Procedure Mark_Invar (Approximation for demonstration)
void Mark_Invar(int en, std::set<int>& bfsOrder, int nblocks, const std::vector<int>& ninsts, std::vector<std::vector<MIRInst>>& Block, std::vector<std::vector<bool>>& InstInvar) {
    int max_inst = 0;
    for (int block_idx = 0; block_idx < nblocks; ++block_idx) {
        max_inst = std::max(max_inst, ninsts[block_idx]);
    }
    InstInvar.resize(nblocks, std::vector<bool>(max_inst, false));

    std::queue<int> q;
    std::vector<bool> visited(nblocks, false);

    q.push(en);
    visited[en] = true;
    bfsOrder.insert(en);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        // Placeholder: Simulate successor finding (replace with actual CFG traversal)
        for (int v = 0; v < nblocks; ++v) {
            if (u != v && !visited[v]) {
                // Assume some arbitrary connection for demonstration
                if ((u + 1) % nblocks == v) {
                    q.push(v);
                    visited[v] = true;
                    bfsOrder.insert(v);
                }
            }
        }
    }

    // Core algorithm of LICM
    bool change = true;
    while (change) {
        change = false;
        for (int block_idx : bfsOrder) {
            for (int inst_idx = 0; inst_idx < ninsts[block_idx]; ++inst_idx) {
                if (!InstInvar[block_idx][inst_idx]) {
                    const MIRInst& inst = Block[block_idx][inst_idx];
                    std::string kind = inst.kind;

                    if (kind == "binexp") { // both binexp operands are constants
                        if (Loop_Const(inst.opd1, block_idx, nblocks, ninsts, Block) &&
                            Loop_Const(inst.opd2, block_idx, nblocks, ninsts, Block)) {
                            InstInvar[block_idx][inst_idx] = true;
                            change = true;
                        }
                    } else if (kind == "unexp") { // unexp operand is constant
                        if (Loop_Const(inst.opd, block_idx, nblocks, ninsts, Block)) {
                            InstInvar[block_idx][inst_idx] = true;
                            change = true;
                        }
                    }
                }
            }
        }
    }
}

// Procedure Mark_Block (Approximation for demonstration)
bool Mark_Block(int en, int i, int nblocks, const std::vector<int>& ninsts, const std::vector<std::vector<MIRInst>>& Block, std::vector<std::vector<bool>>& InstInvar) {
    bool invariant = false;
    for (int inst_idx = 0; inst_idx < ninsts[i]; ++inst_idx) {
        const MIRInst& inst = Block[i][inst_idx];
        std::string kind = inst.kind;

        if (kind == "binexp") {
            if ((Loop_Const(inst.opd1, i, nblocks, ninsts, Block) || Reach_Defs_Out(inst.opd1, i, inst_idx, nblocks, ninsts, Block) || Reach_Defs_In_From_Other_Invariant(inst.opd1, i, inst_idx, nblocks, ninsts, Block)) &&
               (Loop_Const(inst.opd2, i, nblocks, ninsts, Block) || Reach_Defs_Out(inst.opd2, i, inst_idx, nblocks, ninsts, Block) || Reach_Defs_In_From_Other_Invariant(inst.opd2, i, inst_idx, nblocks, ninsts, Block))) {
                InstInvar[i][inst_idx] = true;
                invariant = true;
            }
        } else if (kind == "unexp") {
            if (Loop_Const(inst.opd, i, nblocks, ninsts, Block) && !Reach_Defs_Out(inst.opd, i, inst_idx, nblocks, ninsts, Block) || Reach_Defs_In_From_Other_Invariant(inst.opd, i, inst_idx, nblocks, ninsts, Block)) {
                InstInvar[i][inst_idx] = true;
                invariant = true;
            }
        }
        if (InstInvar[i][inst_idx]) {
            invariant = true; // Instruction is already marked invariant
        }
        if (!inst.left.empty()) {
            // Placeholder: Record definition in loop for Reach_Defs_Out
        }
    }
    return invariant;
}

int main() {
    // Example Usage:
    std::vector<std::vector<MIRInst>> blocks = {
        {
            {"binexp", "a", "b", "", "c"},
            {"binexp", "d", "e", "", "f"},
            {"unexp", "", "", "g", "h"}
        },
        {
            {"binexp", "i", "j", "", "k"},
            {"unexp", "", "", "l", "m"}
        }
    };

    int nblocks = blocks.size();
    std::vector<int> ninsts = {static_cast<int>(blocks[0].size()), static_cast<int>(blocks[1].size())};
    int entry_block = 0; // Example entry block

    std::set<int> in_order;
    std::vector<std::vector<bool>> inst_invar;

    Mark_Invar(entry_block, in_order, nblocks, ninsts, blocks, inst_invar);

    std::cout << "Mark Invariant Instructions (Approximation):\n";
    for (int i = 0; i < nblocks; ++i) {
        std::cout << "Block " << i << ":\n";
        for (int j = 0; j < ninsts[i]; ++j) {
            std::cout << "  Instruction " << j << ": Invariant = " << inst_invar[i][j] << "\n";
        }
    }
    std::cout << "\n";

    std::cout << "Mark Blocks with Loop Invariants (Approximation):\n";
    for (int i = 0; i < nblocks; ++i) {
        bool block_invariant = Mark_Block(entry_block, i, nblocks, ninsts, blocks, inst_invar);
        std::cout << "Block " << i << ": Has Loop Invariant = " << block_invariant << "\n";
    }

    return 0;
}
