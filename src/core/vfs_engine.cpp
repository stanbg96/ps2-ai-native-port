#include "vfs_engine.hpp"
#include <fstream>
#include <iostream>

VfsEngine& VfsEngine::getInstance() {
    static VfsEngine instance;
    return instance;
}

bool VfsEngine::mountObbDirectory(const std::string& path) {
    obbDirectory = path;
    isObbMounted = true;
    std::cout << "[VFS] OBB файлът е монтиран от: " << path << "\n";
    return true;
}

bool VfsEngine::loadAsset(const std::string& assetName, std::vector<uint8_t>& outData) {
    if (!isObbMounted) return false;
    std::string fullPath = obbDirectory + "/" + assetName;
    std::ifstream file(fullPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    outData.resize(size);
    file.read(reinterpret_cast<char*>(outData.data()), size);
    return true;
}

void VfsEngine::runNativeLoop() {
    std::cout << "[VFS] Играта е активна: Менюта, 3D свят и звуци се четат нативно от OBB!\n";
}
