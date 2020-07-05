#pragma once
#include "includes.hpp"
#include "types.hpp"

namespace risc {
    extern std::vector<uint8_t> memory;

    size_type get_index(const cell &c) {
        switch (c.type) {
            case val_types::ABS:
            case val_types::LBL:
            case val_types::REG:
            case val_types::VAL:
                return c.index;
            case val_types::REL:
                return memory[c.index];
            case val_types::NONE:
                throw std::out_of_range("invalid address");
        }
    }

    size_type in(std::array<cell, 3> &cells, size_type pos) {
        int n = 0;
        //scanf("%" SCNu8 "", &n);
        //scanf("\n");
        std::string inp;
        std::getline(std::cin, inp);
        try {
            n = std::stoi(inp);
        }
        catch (std::exception &ex) { }
        memory[get_index(cells[0])] = (uint8_t) n;
        return pos + 1;
    }

    size_type out(std::array<cell, 3> &cells, size_type pos) {
        unsigned n = (unsigned) memory[get_index(cells[0])];
        //printf("%" PRIu8 "\n", n);
        std::cout << n << std::endl;
        return pos + 1;
    }

    size_type bitwise_not(std::array<cell, 3> &cells, size_type pos) {
        size_type a = get_index(cells[0]),
                  b = get_index(cells[1]);
        memory[b] = ~memory[a];
        return pos + 1;
    }

    size_type add(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])],
                b = memory[get_index(cells[1])];
        memory[get_index(cells[2])] = (a + b) % 0x100;
        return pos + 1;
    }

    size_type sub(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])],
                b = memory[get_index(cells[1])];
        memory[get_index(cells[2])] = (a + ~b + 1) % 0x100;
        return pos + 1;
    }

    size_type jz(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])];
        if (a == 0) {
            return get_index(cells[1]);
        }
        else {
            return pos + 1;
        }
    }

    size_type jo(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])];
        if ((a & 0xFF) != 0xFF) {
            return pos + 1;
        }
        else {
            return get_index(cells[1]);
        }
    }

    size_type bitwise_xor(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])],
                b = memory[get_index(cells[1])];
        memory[get_index(cells[2])] = a ^ b;
        return pos + 1;
    }

    size_type ror(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])],
                b = memory[get_index(cells[1])] % 0x8;
        memory[get_index(cells[2])] = (a >> b) | (a << (0x8 - b));
        return pos + 1;
    }

    size_type rol(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])],
                b = memory[get_index(cells[1])] % 0x8;
        memory[get_index(cells[2])] = (a << b) | (a >> (0x8 - b));
        return pos + 1;
    }

    size_type bitwise_or(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])],
                b = memory[get_index(cells[1])];
        memory[get_index(cells[2])] = a | b;
        return pos + 1;
    }

    size_type bitwise_and(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])],
                b = memory[get_index(cells[1])];
        memory[get_index(cells[2])] = a & b;
        return pos + 1;
    }

    size_type bitwise_nand(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])],
                b = memory[get_index(cells[1])];
        memory[get_index(cells[2])] = ~(a & b);
        return pos + 1;
    }

    size_type bitwise_nor(std::array<cell, 3> &cells, size_type pos) {
        uint8_t a = memory[get_index(cells[0])],
                b = memory[get_index(cells[1])];
        memory[get_index(cells[2])] = ~(a | b);
        return pos + 1;
    }
}
