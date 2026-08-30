#include "ps2_engine.hpp"
#include "iso_reader.hpp"
#include "trace_recorder.hpp"
#include <iostream>

PS2Engine& PS2Engine::getInstance() {
    static PS2Engine instance;
    return instance;
}

bool PS2Engine::loadIso(const std::string& isoPath) {
    std::cout << "[PS2 Engine] Стартиране в режим 'ОБУЧЕНИЕ & ПОРТВАНЕ': " << isoPath << "\n";
    TraceRecorder::getInstance().setGameId(isoPath);

    sound.initAudioStream(48000);
    graphics.initVulkanSurface();
    graphics.setTargetRefreshRate(120);

    // Запис на първоначалните Boot код блокове
    std::vector<uint32_t> bootBlock = {0x3C010002, 0x24210010, 0x00221820, 0x03E00008};
    TraceRecorder::getInstance().recordBlock(0x00100000, bootBlock);

    running = true;
    return true;
}

void PS2Engine::stepFrame() {
    if (!running) return;

    // Всяко завъртане на кадъра добавя нови данни към AI модела
    std::vector<uint32_t> currentBlock = {0x24210001, 0x00221820};
    static uint32_t simPC = 0x00100020;
    simPC += 8;
    TraceRecorder::getInstance().recordBlock(simPC, currentBlock);

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
