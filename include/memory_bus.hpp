#pragma once
#include <cstdint>
#include <vector>
#include <cstring>
#include <iostream>

class MemoryBus {
private:
    std::vector<uint8_t> mainRam;      // 32MB PS2 Main RAM
    std::vector<uint8_t> scratchPad;   // 16KB SPRAM (Ultra Fast)
    std::vector<uint8_t> iopRam;       // 2MB IOP RAM

public:
    MemoryBus();
    uint8_t  read8(uint32_t address);
    uint16_t read16(uint32_t address);
    uint32_t read32(uint32_t address);
    
    void write8(uint32_t address, uint8_t value);
    void write16(uint32_t address, uint16_t value);
    void write32(uint32_t address, uint32_t value);

    uint8_t* getFastPointer(uint32_t address);
};
