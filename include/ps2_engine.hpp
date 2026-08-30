#pragma once
#include "memory_bus.hpp"
#include "dma_controller.hpp"
#include "spu2_sound.hpp"
#include "vulkan_bridge.hpp"
#include "profiler.hpp"
#include <string>
#include <vector>
#include <atomic>

struct EngineStats {
    float fps;
    uint32_t compiledBlocks;
    uint32_t drawCalls;
    bool isRunning;
};

class PS2Engine {
private:
    MemoryBus memory;
    DmaController dma;
    Spu2Engine sound;
    VulkanBridge graphics;
    AiProfiler profiler;
    std::atomic<bool> running{false};
    uint32_t activeButtons = 0;

public:
    static PS2Engine& getInstance();
    bool loadIso(const std::string& isoPath);
    void stepFrame();
    void setInputState(uint32_t buttonsMask);
    EngineStats getStats();
    void stop();
};
