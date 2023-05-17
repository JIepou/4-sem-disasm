#include <iostream>
#include "Chip8.h"

int main() {
    std::cout << "CHIP8 Disassembler" << std::endl;
    Chip8 chip;
    std::string filename;
    std::cout << "Enter file to disassemble: " << std::endl;
    std::cin >> filename;
    chip.LoadROM(filename);
    chip.Diassemble();
    return 0;
}