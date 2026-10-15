// Terminal module, which can print and read characters

#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <systemc>
#include <systemc.h>
#include <queue>

using namespace sc_core;

SC_MODULE(TERMINAL) {
    sc_in<bool> clk;
    sc_in<uint32_t> addr;
    sc_in<uint32_t> wdata;
    sc_in<bool> r;
    sc_in<bool> w;

    sc_out<uint32_t> rdata;
    sc_out<bool> ready;
    sc_out<bool> end;


    char terminalToStdout;
    uint32_t latency;
    std::string file_output;

    // Outs and ins counter
    uint32_t outs;
    uint32_t ins;

    // Buffers for read operation
    std::queue<char> input_buffer;
    std::string line_buffer;

    SC_CTOR(TERMINAL);
    TERMINAL(sc_module_name name, char terminalToStdout, uint32_t latency) : sc_module(name), terminalToStdout(terminalToStdout), latency(latency), outs(0), ins(0) {
        SC_THREAD(behaviour);
        sensitive << clk.pos();
    }

    void behaviour() {
        while(true) {
            wait();
            
            // Reset end at beginning of clock-cycle
            end.write(0);
            
            // Check if terminal operation is required
            if (addr.read() == 0xFFFFFFFF) {
                if (r.read()) {
                    doRead(w.read());
                }
                if (w.read()) {
                    doWrite();
                }
            }
            
        }
    }

    void doRead(bool dontSetReady) {
        // Start of read terminal operation
        ready.write(0);
        // Check if buffer needs to be filled
        if (input_buffer.empty()) {
            ins++;
            std::cout << "Please enter terminal input: " << std::flush;
            std::getline(std::cin, line_buffer);
            line_buffer += '\0';

            for (char ch : line_buffer) {
                input_buffer.push(ch);
            }
        }

        // Get next char from buffer
        char input = input_buffer.front();
        input_buffer.pop();

        // Wait according to latency
        for (uint32_t i = 0; i < latency; i++) {
            wait();
        }

        // Write rdata
        rdata.write(input);
        if (!dontSetReady) {
            ready.write(1);
        }
    }

    void doWrite() {
        // Start of write terminal operation
        ready.write(0);

        // Get char from wdata in little endian format
        char input = static_cast<char>(wdata.read() & 0xFF);
        if (input != 0) {
            if (terminalToStdout == 0) {
                // Write on console if --terminale-file flag is not set
                std::cout << input << std::flush;
            } else {
                // Save if --terminal-file flag is set
                file_output += input;
            }
        } else {
            // Set end to 1 and flush stdout
            end.write(1);

            // Count outs
            outs++;
            
            ready.write(1);
            if (terminalToStdout == 0) {
                std::cout << std::endl;
            }
            return;
        }

        // Wait according to latency
        for (uint32_t i = 0; i < latency; i++) {
            wait();
        }
        
        ready.write(1);
    }

    void output(char value) {
        if (terminalToStdout == 0) {
            // Write on console if --terminale-file flag is not set
            std::cout << value << std::flush;
        } else {
             // Save if --terminal-file flag is set
            file_output += value;
        }
    }
};

#endif // TERMINAL_HPP