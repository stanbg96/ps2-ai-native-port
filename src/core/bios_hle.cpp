#include "bios_hle.hpp"

bool BiosHle::handleSyscall(uint32_t syscall_num, uint32_t* gpr) {
    switch (syscall_num) {
        case 0x02: // SetVCommon
        case 0x04: // Exit
            std::cout << "[BIOS HLE] Syscall 0x" << std::hex << syscall_num << " (Exit/VCommon)\n";
            return true;
        case 0x10: // CreateThread
            std::cout << "[BIOS HLE] Създаване на нишка (Thread) за играта\n";
            gpr[3] = 1; // Thread ID = 1
            return true;
        case 0x12: // StartThread
            std::cout << "[BIOS HLE] Стартиране на активната нишка\n";
            return true;
        case 0x3C: // InitHeap
            std::cout << "[BIOS HLE] Инициализиране на системната памет (Heap)\n";
            return true;
        default:
            std::cout << "[BIOS HLE] Прехванат непознат Syscall: 0x" << std::hex << syscall_num << "\n";
            return true;
    }
}
