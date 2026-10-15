// Starts the TinyRISC CPU

#include <systemc>
#include "result.h"
#include "tinyRISC.hpp"


Result run_simulation(uint32_t cycles, const char* tracefile, uint32_t* instructions, size_t instructionCount, char terminalToStdout, uint32_t latency) {
    std::map<uint32_t, uint32_t> initialMemory;
    
    for (size_t i = 0; i < instructionCount; i++) {
        initialMemory[0x00001000 + i * 4] = instructions[i];
    }

    sc_trace_file* file = nullptr;
    if (tracefile != nullptr) {
        file = sc_create_vcd_trace_file(tracefile);
    }

    TINY_RISC tinyRISC("tinyRISC", initialMemory, 1, file, terminalToStdout, latency);

    // Binding the reset signal to a permanent false
    sc_signal<bool> reset_signal;
    reset_signal.write(0);
    tinyRISC.reset.bind(reset_signal);

    std::cout << "\nStarting simulation for " << cycles << " cycles..." << std::endl;

    std::cout << "\n############### TERMINAL ###############" << std::endl;

    sc_start(static_cast<int>(cycles), SC_NS);

    std::cout << "########################################" << std::endl;

    std::cout << "\nSimulation completed successfully." << std::endl;

    if (tracefile != nullptr) {
        sc_close_vcd_trace_file(file);
    }

    // DEBUG
    // for (int i = 0; i < 32; ++i) {
    // std::cout << "x" << i << ": 0x" 
    //             << std::hex << tinyRISC.rf.registers_data[i] 
    //             << std::dec << " (" << tinyRISC.rf.registers_data[i] << ")" 
    //             << std::endl;
    // }
    
    Result result;
    result.cycles = tinyRISC.cycles;
    char* output = new char[tinyRISC.ter.file_output.size() + 1];
    result.output = std::strcpy(output, tinyRISC.ter.file_output.c_str());
    result.outs = tinyRISC.ter.outs;
    result.ins = tinyRISC.ter.ins;
    return result;
}

int sc_main(int argc, char* argv[]) {
    std::cout << "ERROR" << std::endl;
    return 1;
}
