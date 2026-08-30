#include "dma_controller.hpp"

void DmaController::triggerTransfer(DmaChannel channel, uint32_t sourceAddress, uint32_t qwordCount) {
    switch(channel) {
        case DMA_GIF:
            std::cout << "[DMAC] Директен трансфер към Graphics Synthesizer: " 
                      << qwordCount << " QWORDS от адрес 0x" << std::hex << sourceAddress << std::dec << "\n";
            break;
        case DMA_VIF1:
            std::cout << "[DMAC] Трансфер към Vector Unit 1 (VU1 Microcode)\n";
            break;
        case DMA_SIF0:
        case DMA_SIF1:
            std::cout << "[DMAC] SIF Inter-Processor трансфер активен\n";
            break;
        default:
            break;
    }
}
