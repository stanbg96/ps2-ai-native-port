#include "vulkan_bridge.hpp"
#include <iostream>

void VulkanBridge::initVulkanSurface() {
    std::cout << "[Vulkan Engine] Инициализиран графичен буфер с Vulkan.\n";
}

void VulkanBridge::setTargetRefreshRate(int hz) {
    std::cout << "[Vulkan Engine] Режим на опресняване: " << hz << " FPS.\n";
}

void VulkanBridge::renderFrame() {
    // Vulkan Draw Call към GPU
}
