#pragma once
#include <atomic>
#include <thread>
#include <vector>
#include <iostream>

class TurboTranslator {
private:
    std::atomic<bool> isTurboActive{false};
    std::thread workerThread;

public:
    static TurboTranslator& getInstance();
    void startTurboTranslation();
    void stopTurboTranslation();
    bool isActive() const { return isTurboActive.load(); }
};
