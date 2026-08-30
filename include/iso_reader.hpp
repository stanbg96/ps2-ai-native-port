#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class IsoReader {
public:
    static bool parseSystemCnf(const std::string& isoPath, std::string& outBootElf);
    static bool extractBootElf(const std::string& isoPath, const std::string& elfName, std::vector<uint8_t>& outData);
};
