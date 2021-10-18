
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

    Byte operator [](u32 addr) const {
        return data[addr];
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

    Byte fetch (u32 &cycles, Mem &mem) {
        Byte data = mem[pc];
        pc++;
        cycles--;
        return data;
    }

    void execute (u32 cycles, Mem &mem) {
        while (cycles > 0) {
            Byte ins = fetch(cycles, mem);
            (void)ins;
        }
    }
};

int main () {
    Mem mem;
    CPU cpu;
    cpu.reset(mem);
    cpu.execute(2, mem);

    return 0;
}
