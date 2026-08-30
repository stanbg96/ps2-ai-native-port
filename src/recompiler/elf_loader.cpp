#include "elf_loader.hpp"
#include <iostream>
#include <fstream>

bool ElfLoader::loadElf(const std::string& path, std::vector<uint32_t>& outMipsCode) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;
    Elf32_Ehdr header;
    file.read(reinterpret_cast<char*>(&header), sizeof(Elf32_Ehdr));
    return (header.e_ident[0] == 0x7F && header.e_ident[1] == 'E' && header.e_ident[2] == 'L' && header.e_ident[3] == 'F');
}
