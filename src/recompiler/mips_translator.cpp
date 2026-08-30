#include "mips_translator.hpp"
#include <iostream>
#include <sstream>

MipsInstruction MipsTranslator::decode(uint32_t raw) {
    MipsInstruction inst;
    inst.raw = raw;
    inst.opcode = (raw >> 26) & 0x3F;
    inst.rs = (raw >> 21) & 0x1F;
    inst.rt = (raw >> 16) & 0x1F;
    inst.rd = (raw >> 11) & 0x1F;
    inst.funct = raw & 0x3F;
    inst.immediate = raw & 0xFFFF;
    inst.address = raw & 0x03FFFFFF;
    return inst;
}

std::string MipsTranslator::translateToNativeCpp(const MipsInstruction& inst) {
    std::stringstream ss;
    switch (inst.opcode) {
        case 0x00:
            if (inst.funct == 0x20) ss << "cpu.r[" << inst.rd << "] = (int32_t)cpu.r[" << inst.rs << "] + (int32_t)cpu.r[" << inst.rt << "];";
            else if (inst.funct == 0x08) ss << "goto *jump_table[cpu.r[" << inst.rs << "]];";
            else ss << "// R-Type 0x" << std::hex << inst.funct;
            break;
        case 0x09: ss << "cpu.r[" << inst.rt << "] = cpu.r[" << inst.rs << "] + (int16_t)" << (int16_t)inst.immediate << ";"; break;
        case 0x0F: ss << "cpu.r[" << inst.rt << "] = (uint32_t)" << inst.immediate << " << 16;"; break;
        case 0x0C: ss << "cpu.r[" << inst.rt << "] = cpu.r[" << inst.rs << "] & 0x" << std::hex << inst.immediate << ";"; break;
        default: ss << "// Opcode 0x" << std::hex << inst.opcode << " AOT native"; break;
    }
    return ss.str();
}

void MipsTranslator::translateBlock(const std::vector<uint32_t>& mips_code) {
    std::cout << "[Recompiler] Превеждане на PS2 код блокове към ARM64...\n";
    for (uint32_t c : mips_code) {
        MipsInstruction inst = decode(c);
        std::cout << "  0x" << std::hex << c << " -> " << translateToNativeCpp(inst) << "\n";
    }
}
