#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct Elf32_Ehdr {
    unsigned char e_ident[16];
    uint16_t e_type, e_machine;
    uint32_t e_version, e_entry, e_phoff, e_shoff, e_flags;
    uint16_t e_ehsize, e_phentsize, e_phnum, e_shentsize, e_shnum, e_shstrndx;
};

class ElfLoader {
public:
    static bool loadElf(const std::string& path, std::vector<uint32_t>& outMipsCode);
};
