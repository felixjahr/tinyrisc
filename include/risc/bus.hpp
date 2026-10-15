// TinyRISC Update: Bus module to manage r, w, rdata, wdata signals and provide an iosource signal

#ifndef BUS_HPP
#define BUS_HPP

#include <systemc>
#include <systemc.h>
using namespace sc_core;

SC_MODULE(BUS) {
    sc_in<bool> rin;
    sc_in<bool> win;

    sc_in<uint32_t> addrin;
    sc_in<uint32_t> rdatain;
    sc_in<uint32_t> wdatain;

    sc_out<bool> rter;
    sc_out<bool> rmm;
    sc_out<bool> wter;
    sc_out<bool> wmm;

    sc_out<uint32_t> addr;
    sc_out<uint32_t> rdata;
    sc_out<uint32_t> wdata;
    sc_out<uint8_t> iosource;

    SC_CTOR(BUS) {
        SC_THREAD(behavior);
        sensitive << rin << win << addrin << rdatain << wdatain;
    }

    void behavior() {
        while(true) {
            // Reset signals
            rter.write(0);
            rmm.write(0);
            wter.write(0);
            wmm.write(0);

            // Check which IO device is used
            if (addrin.read() == 0xFFFFFFFF) {
                // Set signals for terminal operation
                rter.write(rin.read());
                wter.write(win.read());
                iosource.write(1);
            } else {
                // Set signals for main memory operation
                rmm.write(rin.read());
                wmm.write(win.read());
                iosource.write(0);
            }
            
            // Pass addr, rdata and wdata to IO device
            addr.write(addrin.read());
            rdata.write(rdatain.read());
            wdata.write(wdatain.read());
            
            wait();
        }
    }
};

#endif // BUS_HPP