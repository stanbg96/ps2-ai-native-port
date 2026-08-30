#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct MipsInstruction {
    uint32_t raw, opcode, rs, rt, rd, funct;
    uint16_t immediate;
    uint32_t address;
};

class MipsTranslator {
public:
    MipsInstruction decode(uint32_t instruction);
    std::string translateToNativeCpp(const MipsInstruction& inst);
    void translateBlock(const std::vector<uint32_t>& mips_code);
};
