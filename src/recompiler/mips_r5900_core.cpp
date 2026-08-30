#include "mips_r5900_core.hpp"
#include <sstream>
#include <iostream>

std::string R5900Core::translateInstruction(uint32_t inst, uint32_t current_pc) {
    uint32_t opcode = (inst >> 26) & 0x3F;
    uint32_t rs = (inst >> 21) & 0x1F;
    uint32_t rt = (inst >> 16) & 0x1F;
    uint32_t rd = (inst >> 11) & 0x1F;
    uint32_t funct = inst & 0x3F;
    int16_t imm = inst & 0xFFFF;
    uint32_t target = (inst & 0x03FFFFFF) << 2;

    std::stringstream ss;

    switch (opcode) {
        case 0x00: // SPECIAL
            if (funct == 0x20) { // ADD
                ss << "state.gpr[" << rd << "]._32[0] = state.gpr[" << rs << "]._32[0] + state.gpr[" << rt << "]._32[0];";
            } else if (funct == 0x08) { // JR
                ss << "return state.gpr[" << rs << "]._32[0]; // Branch Delay Handled";
            } else {
                ss << "// R-Type funct 0x" << std::hex << funct;
            }
            break;

        case 0x09: // ADDIU
            ss << "state.gpr[" << rt << "]._32[0] = state.gpr[" << rs << "]._32[0] + (" << imm << ");";
            break;

        case 0x0F: // LUI
            ss << "state.gpr[" << rt << "]._32[0] = (uint32_t)(" << imm << " << 16);";
            break;

        case 0x23: // LW (Load Word)
            ss << "state.gpr[" << rt << "]._32[0] = *(uint32_t*)(memory_map + (state.gpr[" << rs << "]._32[0] + " << imm << "));";
            break;

        case 0x2B: // SW (Store Word)
            ss << "*(uint32_t*)(memory_map + (state.gpr[" << rs << "]._32[0] + " << imm << ")) = state.gpr[" << rt << "]._32[0];";
            break;

        case 0x04: // BEQ (Branch Equal)
            ss << "if (state.gpr[" << rs << "]._32[0] == state.gpr[" << rt << "]._32[0]) goto loc_" << std::hex << (current_pc + 4 + (imm << 2)) << ";";
            break;

        case 0x03: // JAL (Jump and Link)
            ss << "state.gpr[31]._32[0] = 0x" << std::hex << (current_pc + 8) << "; goto loc_" << target << ";";
            break;

        default:
            ss << "// Opcode: 0x" << std::hex << opcode;
            break;
    }

    return ss.str();
}

void R5900Core::compileFunctionBlock(const uint32_t* code, size_t count, uint32_t start_pc) {
    std::cout << "[AOT Recompiler] Превеждане на функция при 0x" << std::hex << start_pc << " (" << std::dec << count << " инструкции)\n";
    for (size_t i = 0; i < count; ++i) {
        uint32_t pc = start_pc + (i * 4);
        std::string cppLine = translateInstruction(code[i], pc);
        std::cout << "  loc_" << std::hex << pc << ": " << cppLine << "\n";
    }
}
