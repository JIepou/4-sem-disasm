#include "Chip8.h"
#include <fstream>
#include <iostream>

void Chip8::LoadROM(std::string filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    size = file.tellg();
    memory = new uint8_t [size];
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(memory), size);
    file.close();
}

void Chip8::Diassemble() {
    std::cout << std::hex;
    for(unsigned long long i = 0; i < size; i+=2){
        opcode = (memory[i] << 8u)  | memory[i+1];
        ((*this).*(table[(opcode & 0xF000u) >> 12u]))();
    }

}

Chip8::Chip8() {

    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::OP_1NNN;
    table[0x2] = &Chip8::OP_2NNN;
    table[0x3] = &Chip8::OP_3XKK;
    table[0x4] = &Chip8::OP_4XKK;
    table[0x5] = &Chip8::OP_5XY0;
    table[0x6] = &Chip8::OP_6XKK;
    table[0x7] = &Chip8::OP_7XKK;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::OP_9XY0;
    table[0xA] = &Chip8::OP_ANNN;
    table[0xB] = &Chip8::OP_BNNN;
    table[0xC] = &Chip8::OP_CXKK;
    table[0xD] = &Chip8::OP_DXYN;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;
    table0[0x0] = &Chip8::OP_00E0;
    table0[0xE] = &Chip8::OP_00EE;
    table8[0x0] = &Chip8::OP_8XY0;
    table8[0x1] = &Chip8::OP_8XY1;
    table8[0x2] = &Chip8::OP_8XY2;
    table8[0x3] = &Chip8::OP_8XY3;
    table8[0x4] = &Chip8::OP_8XY4;
    table8[0x5] = &Chip8::OP_8XY5;
    table8[0x6] = &Chip8::OP_8XY6;
    table8[0x7] = &Chip8::OP_8XY7;
    table8[0xE] = &Chip8::OP_8XYE;
    tableE[0x1] = &Chip8::OP_EXA1;
    tableE[0xE] = &Chip8::OP_EX9E;
    tableF[0x07] = &Chip8::OP_FX07;
    tableF[0x0A] = &Chip8::OP_FX0A;
    tableF[0x15] = &Chip8::OP_FX15;
    tableF[0x18] = &Chip8::OP_FX18;
    tableF[0x1E] = &Chip8::OP_FX1E;
    tableF[0x29] = &Chip8::OP_FX29;
    tableF[0x33] = &Chip8::OP_FX33;
    tableF[0x55] = &Chip8::OP_FX55;
    tableF[0x65] = &Chip8::OP_FX65;
}
// Очистка вывода
void Chip8::OP_00E0() {
    std::cout << "CLS                       "  <<"; Clear the display." << std::endl;
}

// Возврат
void Chip8::OP_00EE() {
    std::cout << "RETURN                    " <<"; Return from a subroutine." << std::endl;
}

// Безусловный переход
void Chip8::OP_1NNN() {
    std::cout << "JP 0x0"  << (opcode & 0x0FFFu) << "                 ; Jump to location" << std::endl;
}

// Вызов процедуры
void Chip8::OP_2NNN() {
    std::cout << "CALL 0x0" << (opcode & 0x0FFFu) << "               ; Call subroutine" << std::endl;
}

// Пропускает следующую инструкцию, если VX равно NN
void Chip8::OP_3XKK() {
    std::cout << "SE V"<<((opcode & 0x0F00u) >> 8u) << ", "<<(opcode & 0x00FFu) << "                  ; Skip next instruction if Vx = kk" << std::endl;
}

// Пропускает следующую инструкцию, если VX не равно NN
void Chip8::OP_4XKK() {
    std::cout << "SNE V"<<((opcode & 0x0F00u) >> 8u) << ", "<<(opcode & 0x00FFu) << "                 ; Skip next instruction if Vx != kk" << std::endl;
}

// Пропускает следующую инструкцию, если VX равно VY
void Chip8::OP_5XY0() {

    std::cout << "SE V"<<((opcode & 0x0F00u) >> 8u) << ", V"<<((opcode & 0x00F0u) >> 4u)<< "                 ; Skip next instruction if Vx = Vy" << std::endl;
}

// Установить Vx = kk.
void Chip8::OP_6XKK() {
    std::cout << "LD V"<<((opcode & 0x0F00u) >> 8u) << ", 0x00"<<((opcode & 0x00FFu))<< "              ; Set Vx = kk" << std::endl;
}

// Установить Vx = Vx + kk.
void Chip8::OP_7XKK() {
    std::cout << "ADD V"<<((opcode & 0x0F00u) >> 8u) << ", 0x00"<<((opcode & 0x00FFu))<< "            ; Set Vx = Vx + kk" << std::endl;
}

// Установить Vx = Vy
void Chip8::OP_8XY0() {
    std::cout << "LD V"<<((opcode & 0x0F00u) >> 8u) << ", V"<<((opcode & 0x00F0u) >> 4u)<< "                 ; Set Vx = Vy" << std::endl;
}

// Установить Vx = Vx ИЛИ Vy
void Chip8::OP_8XY1() {
    std::cout << "OR V"<<((opcode & 0x0F00u) >> 8u) << ", V"<<((opcode & 0x00F0u) >> 4u)<< "                 ; Set Vx = Vx OR Vy" << std::endl;
}

// Установить Vx = Vx И Vy
void Chip8::OP_8XY2() {
    std::cout << "AND V"<<((opcode & 0x0F00u) >> 8u) << ", V"<<((opcode & 0x00F0u) >> 4u)<< "                 ; Set Vx = Vx AND Vy" << std::endl;
}

// Установить Vx = Vx XOR Vy
void Chip8::OP_8XY3() {
    std::cout << "XOR V"<<((opcode & 0x0F00u) >> 8u) << ", V"<<((opcode & 0x00F0u) >> 4u)<< "                 ; Set Vx = Vx XOR Vy" << std::endl;
}

// Установить Vx = Vx + Vy, установить VF = перенос. В в 0xF. Если сумма больше 255, сколько 8 бит может содержать максимум, то она переносится.
void Chip8::OP_8XY4() {
    std::cout << "ADD V"<<((opcode & 0x0F00u) >> 8u) << ", V"<<((opcode & 0x00F0u) >> 4u)<< "                 ; Set Vx = Vx + Vy, VF = carry" << std::endl;
}

// Установить Vx = Vx - Vy, установить VF = НЕ заимствовать.
void Chip8::OP_8XY5() {
    std::cout << "SUB V"<<((opcode & 0x0F00u) >> 8u) << ", V"<<((opcode & 0x00F0u) >> 4u)<< "                 ; Set Vx = Vx - Vy, VF = NOT borrow" << std::endl;
}

// Сохраняет младший значащий бит VX в VF, а затем сдвигает VX вправо на 1.
void Chip8::OP_8XY6() {
    std::cout << "SHR V"<<((opcode & 0x0F00u) >> 8u) << "{, V"<<((opcode & 0x00F0u) >> 4u)<< "}                 ; Set Vx = Vx SHR 1" << std::endl;
}

// Установить Vx = Vy - Vx, установить VF = НЕ заимствовать.
void Chip8::OP_8XY7() {
    std::cout << "SUBN V"<<((opcode & 0x0F00u) >> 8u) << ", V"<<((opcode & 0x00F0u) >> 4u)<< "                ; Set Vx = Vy - Vx, VF = NOT borrow" << std::endl;
}

// Сохраняет старший бит VX в VF, а затем сдвигает VX влево на 1
void Chip8::OP_8XYE() {
    std::cout << "SHL V"<<((opcode & 0x0F00u) >> 8u) << "{, V"<<((opcode & 0x00F0u) >> 4u)<< "}                 ; Set Vx = Vx SHL 1" << std::endl;
}

// Пропустить следующую инструкцию, если Vx != Vy.
void Chip8::OP_9XY0() {
    std::cout << "SNE V"<<((opcode & 0x0F00u) >> 8u) << ", V"<<((opcode & 0x00F0u) >> 4u)<< "                 ; Skip next instruction if Vx != Vy" << std::endl;
}


// Устанавливает I по адресу NNN. индекс
void Chip8::OP_ANNN() {
    std::cout << "LD I"<<((opcode & 0x0F00u) >> 8u) << ", 0x0"<<((opcode & 0x0FFFu))<< "              ; Set I = nnn" << std::endl;
}

// Перейти к местоположению nnn + V0.
void Chip8::OP_BNNN() {
    std::cout << "JP V0" << ", 0x0"<<((opcode & 0x0FFFu))<< "              ; Jump to location nnn + V0" << std::endl;
}

// Установить Vx = случайный байт И kk.
void Chip8::OP_CXKK() {
    std::cout << "RND V"<<((opcode & 0x0F00u) >> 8u) << ", 0x00"<<((opcode & 0x00FFu))<< "              ; Set Vx = random byte AND kk" << std::endl;
}

// Отобразите n-байтовый спрайт, начиная с ячейки памяти I в (Vx, Vy), установите VF = столкновение.
// Интерпретатор считывает n байтов из памяти, начиная с адреса, хранящегося в I.
// Затем эти байты отображаются в виде спрайтов на экране с координатами (Vx, Vy).
// Спрайты объединяются XOR с существующим экраном. Если это вызывает стирание каких-либо пикселей, VF устанавливается на 1, в противном случае — на 0.
// Если спрайт расположен так, что его часть находится за пределами координат дисплея, он переносится на противоположную сторону экрана.
void Chip8::OP_DXYN() {
    std::cout << "DRW V"<<((opcode & 0x0F00u) >> 8u) << ", V"<<((opcode & 0x00F0u) >> 4u)<<", 0x000"<<(opcode & 0x000Fu)<< "        ; Display draw n-byte sprite" << std::endl;
}

// Пропустить следующую инструкцию, если нажата клавиша со значением Vx.
void Chip8::OP_EX9E() {
    std::cout << "SKP V"<<((opcode & 0x0F00u) >> 8u) << "                    ; Skip next instruction if key with the value of Vx is pressed" << std::endl;
}

// Пропустить следующую инструкцию, если не нажата клавиша со значением Vx.
void Chip8::OP_EXA1() {
    std::cout << "SKNP V"<<((opcode & 0x0F00u) >> 8u) << "                   ; Skip next instruction if key with the value of Vx is not pressed" << std::endl;
}

// Установить Vx = значение таймера задержки.
void Chip8::OP_FX07() {
    std::cout << "LD V"<<((opcode & 0x0F00u) >> 8u) << ", DT" << "                 ; Set Vx = delay timer value." << std::endl;
}

// Дождитесь нажатия клавиши, сохраните значение клавиши в Vx.
void Chip8::OP_FX0A() {
    std::cout << "LD V"<<((opcode & 0x0F00u) >> 8u) << ", K" << "                  ; Wait for a key press, store the value of the key in Vx." << std::endl;
}

// Установить таймер задержки = Vx.
void Chip8::OP_FX15() {
    std::cout << "LD DT" << ", V"<<((opcode & 0x0F00u) >> 8u) << "                 ; Set delay timer = Vx." << std::endl;

}

// Установите звуковой таймер = Vx.
void Chip8::OP_FX18() {
    std::cout << "LD ST" << ", V"<<((opcode & 0x0F00u) >> 8u) << "                 ; Set sound timer = Vx." << std::endl;

}

// Установите I = I + Vx.
void Chip8::OP_FX1E() {
    std::cout << "ADD I" << ", V"<<((opcode & 0x0F00u) >> 8u) << "                 ; Set I = I + Vx." << std::endl;
}

// Установите I = местоположение спрайта для цифры Vx.
void Chip8::OP_FX29() {
    std::cout << "LD F" << ", V"<<((opcode & 0x0F00u) >> 8u) << "                  ; Set I = location of sprite for digit Vx." << std::endl;

}

// Сохраняем двоично-десятичное представление Vx в ячейках памяти I, I+1 и I+2.
// Интерпретатор берет десятичное значение Vx и помещает разряд сотен в память в ячейку I,
// цифра десятков в ячейке I+1 и цифра единиц в ячейке I+2.
void Chip8::OP_FX33() {
    std::cout << "LD B" << ", V"<<((opcode & 0x0F00u) >> 8u) << "                  ; Store BCD representation of Vx in memory locations I, I+1, and I+2." << std::endl;
}

// Сохраните регистры с V0 по Vx в памяти, начиная с ячейки I.
void Chip8::OP_FX55() {
    std::cout << "LD [I]" << ", V"<<((opcode & 0x0F00u) >> 8u) << "                 ; Store registers V0 through Vx in memory starting at location I." << std::endl;
}

// Считать регистры с V0 по Vx из памяти, начиная с ячейки I.
void Chip8::OP_FX65() {
    std::cout << "LD V"<<((opcode & 0x0F00u) >> 8u) << ", [I]" << "                ; Read registers V0 through Vx from memory starting at location I." << std::endl;
}



// Таблица
void Chip8::Table0(){
    ((*this).*(table0[opcode & 0x000Fu]))();
}

void Chip8::Table8() {
    ((*this).*(table8[opcode & 0x000Fu]))();
}

void Chip8::TableE() {
    ((*this).*(tableE[opcode & 0x000Fu]))();
}

void Chip8::TableF() {
    ((*this).*(tableF[opcode & 0x00FFu]))();
}

