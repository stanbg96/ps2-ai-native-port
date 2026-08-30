#pragma once
#include <cstdint>
#include <iostream>

enum DmaChannel {
    DMA_VIF0 = 0,
    DMA_VIF1 = 1,
    DMA_GIF  = 2,
    DMA_IPU_FROM = 3,
    DMA_IPU_TO   = 4,
    DMA_SIF0 = 5,
    DMA_SIF1 = 6,
    DMA_SIF2 = 7,
    DMA_SPR_FROM = 8,
    DMA_SPR_TO   = 9
};

class DmaController {
public:
    void triggerTransfer(DmaChannel channel, uint32_t sourceAddress, uint32_t qwordCount);
};
