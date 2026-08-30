#pragma once
#include <string>
#include <vector>
#include <iostream>

class VfsEngine {
private:
    std::string obbDirectory;
    bool isObbMounted = false;

public:
    static VfsEngine& getInstance();
    bool mountObbDirectory(const std::string& path);
    bool loadAsset(const std::string& assetName, std::vector<uint8_t>& outData);
    bool isMounted() const { return isObbMounted; }
};
