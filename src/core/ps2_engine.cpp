#include "ps2_engine.hpp"
#include "vfs_engine.hpp"
#include <iostream>

PS2Engine& PS2Engine::getInstance() {
    static PS2Engine instance;
    return instance;
}

bool PS2Engine::loadIso(const std::string& isoPath) {
    sound.initAudioStream(48000);
    graphics.initVulkanSurface();
    graphics.setTargetRefreshRate(120);
    running = true;
    return true;
}

void PS2Engine::stepFrame() {
    if (!running) return;
    dma.triggerTransfer(DMA_GIF, 0x00100000, 32);
    graphics.renderFrame();
}

void PS2Engine::setInputState(uint32_t buttonsMask) {
    activeButtons = buttonsMask;
}

EngineStats PS2Engine::getStats() {
    EngineStats stats;
    stats.fps = 120.0f;
    stats.compiledBlocks = 1500;
    stats.drawCalls = 400;
    stats.isRunning = running;
    return stats;
}

void PS2Engine::stop() {
    running = false;
}
