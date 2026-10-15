// Control unit for instruction fetch, decode, execution, and write-back.

#ifndef CU_HPP
#define CU_HPP

#include <systemc>
#include <systemc.h>
using namespace sc_core;

SC_MODULE(CU) {
  sc_in<bool> clk;
  sc_in<uint32_t> datain;
  sc_in<bool> ready;
  sc_in<uint32_t> ro;
  sc_in<bool> overflow;

  sc_out<bool> pcenable;
  sc_out<sc_bv<2>> pcj;
  sc_out<sc_bv<12>> pcoffset;
  sc_out<bool> r;
  sc_out<bool> w;
  sc_out<sc_bv<3>> op;
  sc_out<sc_bv<5>> a1;
  sc_out<sc_bv<5>> a2;
  sc_out<sc_bv<5>> a3;
  sc_out<bool> we;
  sc_out<uint32_t> immout;
  sc_out<uint8_t> rsource;
  // Control addrin of bus
  sc_out<uint8_t> asource;

  uint32_t imm;
  uint8_t lead;
  uint8_t rs1;
  uint8_t rs2;
  uint8_t rd;
  uint8_t funct;
  uint8_t opcode;

  // Jal instructions
  uint8_t mid;
  uint32_t offset;

  bool lastOverflow;

  SC_CTOR(CU) {
    SC_THREAD(behaviour);
    sensitive << clk.pos();
  }

  void behaviour() {
    while(true) {
      wait();
      wait(SC_ZERO_TIME);
      we.write(false);
      pcenable.write(false);
      pcj.write(false);

      instructionFetch();
      instructionDecode();
      execution();
      memoryAccess();
      writeBack();
    }
  }

  void instructionFetch() {
    // Added SC_ZERO_TIME
    wait(SC_ZERO_TIME);
    wait(SC_ZERO_TIME);

    // Set asource so that pc will be used for addr
    asource.write(0);
    r.write(true);
    
    do {
      wait();
      wait(SC_ZERO_TIME);
      // Added SC_ZERO_TIME
      wait(SC_ZERO_TIME);
      wait(SC_ZERO_TIME);
      wait(SC_ZERO_TIME);
      wait(SC_ZERO_TIME);
    } while(!ready.read());
    
    r.write(false);
  }

  void instructionDecode() {
    wait(SC_ZERO_TIME);

    uint32_t instruction = datain.read();

    opcode = instruction & 0b1111111;
    rd = (instruction >> 7) & 0b11111;

    if (opcode == 0b1101111) {
      // JAL instruction
      mid = (instruction >> 12) & 0b11111111;
      offset = (instruction >> 20) & 0b111111111111;
    } else if (opcode == 0b0100010) {
      // LI instruction
      imm = (instruction >> 12) & 0b11111111111111111111;
    } else {
      funct = (instruction >> 12) & 0b111;
      rs1 = (instruction >> 15) & 0b11111;
      rs2 = (instruction >> 20) & 0b11111;
      lead = (instruction >> 25) & 0b1111111;

      a1.write(rs1);
      a2.write(rs2);
    }
    
    if(opcode == 0b1100011 && funct == 0) {
      a1.write(rd);
    }

    wait();
  }

  void execution() {
    if(opcode == 0b110011) {
      op.write(funct);
    }
    else if(opcode == 0b1100011) {
      op.write(5);
    }

    wait();
  }

  void memoryAccess() {
    if(opcode == 0b100011) {
      asource.write(1);

      r.write(funct == 0);
      w.write(funct == 1);

      do {
        wait();
        wait(SC_ZERO_TIME);
        // Added SC_ZERO_TIME
        wait(SC_ZERO_TIME);
      } while(!ready.read());

      r.write(0);
      w.write(0);
    }
  }

  void writeBack() {
    if(opcode == 0b110011) {
      we.write(true);
      a3.write(rd);
      rsource.write(0);
      lastOverflow = overflow.read();
    } else if(opcode == 0b1100011) {
      uint32_t result = ro.read();
      if(funct == 0) {
        pcoffset.write(0);
        pcj.write(1);
      } else if((funct == 1 && result == 0) || 
                (funct == 2 && result != 0) || 
                (funct == 3 && result > 0x80000000) ||
                (funct == 4 && result < 0x80000000)) {
        pcoffset.write(rd | (lead << 5));
        pcj.write(2);
      } else {
        pcoffset.write(0);
        pcj.write(0);
      }
    } else if(opcode == 0b100011 && funct == 0) {
      we.write(true);
      a3.write(rd);
      rsource.write(1);
    } else if(opcode == 0b1101111) {
      // JAL instruction
      we.write(true);
      a3.write(rd);
      rsource.write(2);

      pcoffset.write(offset);
      pcj.write(2);
    } else if (opcode == 0b0100010) {
      // LI instruction
      we.write(true);
      a3.write(rd);
      immout.write(imm);
      rsource.write(3);
    }
    pcenable.write(true);
  }
};

#endif // CU_HPP