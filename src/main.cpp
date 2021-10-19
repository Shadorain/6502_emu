
#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem {
    static constexpr u32 MAX_MEM = 1024*64;
    Byte data[MAX_MEM];

    void initialize () {
        for (u32 i = 0; i < MAX_MEM; i++) data[i] = 0;
    }

    /* read 1B */
    Byte operator [](u32 addr) const {
        return data[addr];
    }

    /* write 1B */
    Byte& operator [](u32 addr) {
        return data[addr];
    }

    void write_word (Word val, u32 addr, u32& cycles) {
        val[addr] = val & 0xFF;
        val[addr+1] = (val >> 8);
        cycles -= 2;
    }
};

struct CPU {
    Word pc; /* program counter */
    Word sp; /* stack pointer */

    Byte a, x, y; /* registers */

    /* status flags */
    Byte c : 1;
    Byte z : 1;
    Byte i : 1;
    Byte d : 1;
    Byte b : 1;
    Byte v : 1;
    Byte n : 1;

    void reset (Mem &mem) {
        pc = 0xFFFC;
        sp = 0x0100;
        c = z = i = d = b = v = n = 0;
        a = x = y = 0;
        mem.initialize();
    }

    Byte fetch_byte (u32 &cycles, Mem &mem) {
        Byte data = mem[pc];
        pc++;
        cycles--;
        return data;
    }

    Byte fetch_word (u32 &cycles, Mem &mem) {
        Word data = mem[pc];
        pc++;

        data |= (mem[pc] << 8);
        pc++;
        cycles -= 2;

        return data;
    }

    Byte read_byte (u32 &cycles, Byte addr, Mem &mem) {
        Byte data = mem[addr];
        cycles--;
        return data;
    }

    void lda_set_status () {
        z = (a == 0);
        n = (a & 0b10000000) > 0;
    }

    /* opcodes */
    static constexpr Byte INS_LDA_IM = 0xA9,
                          INS_LDA_ZP = 0xA5,
                          INS_LDA_ZPX = 0xB5,
                          INS_JSR = 0x20;

    void execute (u32 cycles, Mem &mem) {
        while (cycles > 0) {
            Byte ins = fetch_byte(cycles, mem);
            switch (ins) {
                case INS_LDA_IM: {
                    Byte val = fetch_byte(cycles, mem);
                    a = val;
                    lda_set_status();
                } break;
                case INS_LDA_ZP: {
                    Byte zp_addr = fetch_byte(cycles, mem);
                    a = read_byte(cycles, zp_addr, mem);
                    lda_set_status();
                } break;
                case INS_LDA_ZPX: {
                    Byte zp_addr = fetch_byte(cycles, mem);
                    zp_addr += x;
                    cycles--;
                    a = read_byte(cycles, zp_addr, mem);
                    lda_set_status();
                } break;
                case INS_JSR: {
                    Word sub_addr = fetch_word(cycles, mem);
                    mem.write_word(pc-1, sp, cycles);
                    pc = sub_addr;
                    cycles--;
                } break;
                default:
                    printf("Instruction not handled %d", ins);
                    break;
            }
        }
    }
};

int main () {
    Mem mem;
    CPU cpu;
    cpu.reset(mem);
    mem[0xFFFC] = CPU::INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = CPU::INS_LDA_IM;
    mem[0x4243] = 0x84;
    cpu.execute(9, mem);

    return 0;
}
