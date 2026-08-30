#pragma once
#include <iostream>

class VulkanBridge {
public:
    void initVulkanSurface();
    void setTargetRefreshRate(int hz = 120);
    void renderFrame();
};
