#pragma once
#include <cstdint>
#include <string>
#include <iostream>

class BiosHle {
public:
    static bool handleSyscall(uint32_t syscall_num, uint32_t* gpr);
};
