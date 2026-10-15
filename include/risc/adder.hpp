// Adder module for PC + 4 calculation

#ifndef ADDER_HPP
#define ADDER_HPP

#include <systemc>
#include "systemc.h"
using namespace sc_core;

SC_MODULE(ADDER) {
  sc_in<uint32_t> inA;
  sc_in<uint32_t> inB;
  sc_out<uint32_t> out;

  SC_CTOR(ADDER) {
    SC_THREAD(behaviour);
    sensitive << inA << inB;
  }

  void behaviour() {
    while(true) {
      out.write(inA.read() + inB.read());
      wait();
    }
  }
};

#endif // ADDER_HPP