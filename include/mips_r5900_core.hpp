#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

// 128-битов регистър на Emotion Engine (EE)
union Register128 {
    uint64_t _64[2];
    uint32_t _32[4];
    uint16_t _16[8];
    uint8_t  _8[16];
};

struct R5900State {
    Register128 gpr[32]; // 32 броя 128-битови регистри ($zero .. $ra)
    uint32_t pc;         // Program Counter
    uint32_t hi, lo;
    uint32_t hi1, lo1;
};

class R5900Core {
public:
    static std::string translateInstruction(uint32_t inst, uint32_t current_pc);
    static void compileFunctionBlock(const uint32_t* code, size_t count, uint32_t start_pc);
};
