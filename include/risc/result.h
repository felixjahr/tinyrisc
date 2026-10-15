// Handles transition between C and C++

#ifndef RESULT_H
#define RESULT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t cycles;
    char* output;
    uint32_t outs;
    uint32_t ins;
} Result;

Result run_simulation(uint32_t cycles, const char* tracefile, uint32_t* instructions, size_t instructionCount, char terminalToStdout, uint32_t latency);

#ifdef __cplusplus
}
#endif

#endif