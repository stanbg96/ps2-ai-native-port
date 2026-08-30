#include "memory_bus.hpp"

MemoryBus::MemoryBus() {
    mainRam.resize(32 * 1024 * 1024, 0);   // 32 MB
    scratchPad.resize(16 * 1024, 0);       // 16 KB
    iopRam.resize(2 * 1024 * 1024, 0);     // 2 MB
    std::cout << "[Memory Engine] 32MB PS2 FastMem шина инициализирана успешно!\n";
}

uint8_t* MemoryBus::getFastPointer(uint32_t address) {
    uint32_t physical = address & 0x1FFFFFFF;
    if (physical < 0x02000000) {
        return &mainRam[physical];
    }
    if (address >= 0x70000000 && address < 0x70004000) {
        return &scratchPad[address - 0x70000000];
    }
    return nullptr;
}

uint32_t MemoryBus::read32(uint32_t address) {
    uint8_t* ptr = getFastPointer(address);
    if (ptr) return *reinterpret_cast<uint32_t*>(ptr);
    return 0;
}

void MemoryBus::write32(uint32_t address, uint32_t value) {
    uint8_t* ptr = getFastPointer(address);
    if (ptr) *reinterpret_cast<uint32_t*>(ptr) = value;
}

uint8_t MemoryBus::read8(uint32_t address) {
    uint8_t* ptr = getFastPointer(address);
    return ptr ? *ptr : 0;
}

void MemoryBus::write8(uint32_t address, uint8_t value) {
    uint8_t* ptr = getFastPointer(address);
    if (ptr) *ptr = value;
}

uint16_t MemoryBus::read16(uint32_t address) {
    uint8_t* ptr = getFastPointer(address);
    return ptr ? *reinterpret_cast<uint16_t*>(ptr) : 0;
}

void MemoryBus::write16(uint32_t address, uint16_t value) {
    uint8_t* ptr = getFastPointer(address);
    if (ptr) *reinterpret_cast<uint16_t*>(ptr) = value;
}
