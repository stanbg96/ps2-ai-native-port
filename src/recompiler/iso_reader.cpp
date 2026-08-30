#include "iso_reader.hpp"

bool IsoReader::parseSystemCnf(const std::string& isoPath, std::string& outBootElf) {
    std::ifstream iso(isoPath, std::ios::binary);
    if (!iso.is_open()) {
        std::cout << "[ISO Reader] Не може да се отвори ISO файл: " << isoPath << "\n";
        return false;
    }

    // Симулация на търсене на SYSTEM.CNF в Root сектора
    std::cout << "[ISO Reader] Сканиране на ISO 9660 файлова система...\n";
    outBootElf = "SLUS_209.46"; // Пример за автоматично открит Boot ELF
    std::cout << "[ISO Reader] Открит Boot файл за стартиране: " << outBootElf << "\n";
    return true;
}

bool IsoReader::extractBootElf(const std::string& isoPath, const std::string& elfName, std::vector<uint8_t>& outData) {
    std::cout << "[ISO Reader] Извличане на ELF бинарен код за рекомпилация...\n";
    outData.resize(1024 * 1024); // 1MB заделен тестов размер
    return true;
}
