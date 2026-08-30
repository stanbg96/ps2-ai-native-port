#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>

struct ExecutionTrace {
    uint32_t start_pc;
    std::vector<uint32_t> mips_opcodes;
    uint64_t hit_count;
};

class TraceRecorder {
private:
    std::unordered_map<uint32_t, ExecutionTrace> recordedBlocks;
    std::string currentGameId;
    size_t totalInstructionsTraced = 0;

public:
    static TraceRecorder& getInstance();
    void setGameId(const std::string& id);
    void recordBlock(uint32_t pc, const std::vector<uint32_t>& opcodes);
    float getPortingProgressPercentage();
    void exportNativeCodebase(const std::string& outputCppPath);
};
