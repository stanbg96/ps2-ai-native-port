#pragma once
#include <cstdint>
#include <unordered_map>
#include <iostream>

class AiProfiler {
private:
    std::unordered_map<uint32_t, uint64_t> executionCount;

public:
    void recordBlockExecution(uint32_t pc) {
        executionCount[pc]++;
    }

    bool isHotPath(uint32_t pc) {
        return executionCount[pc] > 1000;
    }

    void printStats() {
        std::cout << "[AI Profiler] Профилирани активни код блокове: " << executionCount.size() << "\n";
    }
};
