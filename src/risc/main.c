// Frame program for TinyRISC CPU

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>


#include "result.h"

int parse_cli(int argc, char** argv, const char** terminal_file, uint32_t* latency, uint32_t* cycles, const char** tf, const char** input_file) {

    // Defining values for options
    enum {
        OPT_TERMINAL_FILE = 1000,
        OPT_LATENCY = 1001,
        OPT_CYCLES = 1002,
        OPT_TF = 1003,
        OPT_HELP = 1004
    };

    static struct option long_options[] = {
        {"terminal-file", required_argument, 0, OPT_TERMINAL_FILE},
        {"latency", required_argument, 0, OPT_LATENCY},
        {"cycles", required_argument, 0, OPT_CYCLES},
        {"tf", required_argument, 0, OPT_TF},
        {"help", no_argument, 0, OPT_HELP},
        {0, 0, 0, 0}
    };

    int opt;

    // Loop through all command-line options using getopt_long
    while ((opt = getopt_long(argc, argv, "t:l:c:f:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 't':
            case OPT_TERMINAL_FILE:
                // Store path for terminal output file
                *terminal_file = optarg;
                break;
            case 'l':
            case OPT_LATENCY: {
                // Parse and validate latency value
                char *endptr;
                unsigned long tmp = strtoul(optarg, &endptr, 10);
                if (*endptr != '\0') {
                    fprintf(stderr, "Latency \"%s\" is not a valid number\n", optarg);
                    return 1;
                }
                *latency = (uint32_t)tmp;
                break;
            }
            case 'c':
            case OPT_CYCLES: {
                // Parse and validate cycles value
                char *endptr;
                unsigned long tmp = strtoul(optarg, &endptr, 10);
                if (*endptr != '\0') {
                    fprintf(stderr, "Cycles \"%s\" is not a valid number\n", optarg);
                    return 1;
                }
                *cycles = (uint32_t)tmp;
                break;
            }
            case 'f':
            case OPT_TF:
                // Store path for the trace file
                *tf = optarg;
                break;
            case 'h':
            case OPT_HELP:
                // Print help message
                printf("================ USAGE =================\n");
                printf("%s [options] <input_file>\n", argv[0]);
                printf("Options:\n");
                printf("-t, --terminal-file     Specifies the path to be used for the terminal output. If this option is not provided, the terminal writes to stdout.\n");
                printf("-l, --latency           Specifies the number of clock cycles required to output a value to the terminal.\n");
                printf("-c, --cycles            The number of cycles to be simulated.\n");
                printf("-f, --tf                The path to the tracefile. If this option is not set, no tracefile will be created.\n");
                printf("-h, --help              Show this help message\n");
                printf("========================================\n");
                return 0;
            case '?':
                // Handle unknown options detected by getopt_long
                fprintf(stderr, "Unknown option: -%c\n", optopt);
                return 1;

            default:
                // Catch any unexpected errors during option parsing
                fprintf(stderr, "Unexpected error while parsing options.\n");
                return 1;
        }
    }

    // After parsing options, check for mandatory input file
    // optind is the index of the first non optional argument
    if (optind >= argc) {
        fprintf(stderr, "No input file specified.\n");
        return 1;
    }

    // Check if only input file is specified
    while (optind < argc) {
        if (*input_file != NULL) {
            fprintf(stderr, "Too many input files specified.\n");
            return 1;
        }
        *input_file = argv[optind];
        optind++;
    }

    // All arguments parsed successfully
    return 0;
}


int read_binary_file(const char* path, uint32_t** instructions, size_t* count) {
    printf("Reading binary file...\n");

    FILE* file;
    // Attempt to open the file in read mode
    if (!(file = fopen(path, "r"))) {
        perror("Error opening binary file\n");
        return 1;
    }

    // Read the file stats into statbuf
    struct stat statbuf;
    if (fstat(fileno(file), &statbuf)) {
        fprintf(stderr, "Error stating file\n");
        fclose(file);
        return 1;
    }

    // Initialize capacity for instruction array with a starting size
    size_t cap = 16;
    size_t n = 0;
    *instructions = (uint32_t*) malloc(cap * sizeof(uint32_t));
    if (!*instructions) {
        fclose(file);
        return 1;
    }

    // Allocate char array for a line (32 characters of the instruction + 1 \n for + 1 for \0)
    char line[34];

    // Read lines
    while(fgets(line, sizeof line, file)) {
        // Define actual length of the line, including \n
        size_t len = strcspn(line, "\n");
        // Validate line length, must be 32 characters long
        if (len != 32) {
            fprintf(stderr, "Invalid length (length %zu instead of 32): %s\n", len, line);
            free(*instructions);
            fclose(file);
            return 1;
        }

        // Convert binary string to a uint32_t
        uint32_t value = 0;
        for (size_t i = 0; i < 32; i++) {
            char c = line[i];
            // Character must be '0' or '1'
            if (c != '0' && c != '1') {
                fprintf(stderr, "Invalid character '%c' in line: %s\n", c, line);
                free(*instructions);
                fclose(file);
                return 1;
            }
            value = (value << 1) | (c - '0');
        }
        
        // Dynamic array resizing: double if capacity is reached
        if (n == cap) {
            cap *= 2;
            uint32_t *tmp = (uint32_t*) realloc(*instructions, cap * sizeof(uint32_t));
            if (!tmp) {
                fprintf(stderr, "Out of memory\n");
                free(*instructions);
                fclose(file);
                return 5;
            }
            *instructions = tmp;
        }
        // Store the instruction and increment counter
        (*instructions)[n++] = value;
    }    
    
    fclose(file);
    // Set total number of instructions read
    *count = n;
    printf("Successfully read %zu instructions from binary file.\n", *count);
    return 0;
}



int main (int argc, char** argv) {
    // Initialize default values for options
    const char* terminal_file = NULL;
    uint32_t latency = 100;
    uint32_t cycles = 10000;
    const char* tf = NULL;
    const char* input_file = NULL;

    // Parse command-line arguments
    int parse_cli_result = parse_cli(argc, argv, &terminal_file, &latency, &cycles, &tf, &input_file);
    if (input_file == NULL || parse_cli_result != 0) {
        return parse_cli_result;
    }

    // Print confirmation of parsed command-line options
    printf("Command-line options parsed successfully.\n\n");    
    if (terminal_file != NULL) {
        printf("Terminal output will be written to: %s\n", terminal_file);
    } else {
        printf("Terminal output will be written to: stdout\n");
    }
    printf("Latency set to: %u cycles\n", latency);
    printf("Maximum number of cycles: %u\n", cycles);
    if (tf != NULL) {
        printf("Tracefile path: %s\n", tf);
    } else {
        printf("No tracefile path specified. Tracefile will not be generated.\n");
    }
    
    // Read instructions from the input file
    uint32_t* instructions;
    size_t instructionCount;
    if (input_file != NULL) {
        printf("\nReading instructions from input file: %s\n", input_file);
    }
    int read_binary_file_result = read_binary_file(input_file, &instructions, &instructionCount);
    if (read_binary_file_result != 0) {
        return read_binary_file_result;
    }

    // Determine how the simulation function should handle terminal output
    // 0: print to stdout
    // 1: return output string for main to write to file
    char terminalToStdout;
    if (terminal_file == NULL) {
        terminalToStdout = 0;
    } else {
        terminalToStdout = 1;
    }
    
    // Start the simulation by invoking the external C++ run_simulation function
    Result result = run_simulation(cycles, tf, instructions, instructionCount, terminalToStdout, latency);

    // Write collected output to the terminal file, if specified
    if (terminal_file != NULL) {
        printf("Writing terminal output to file: %s\n", terminal_file);
        FILE *file;
        // Open the terminal file in write mode
        if (!(file = fopen(terminal_file, "w"))) {
            perror("Error opening terminal file\n");
            free(result.output);
            return 1;
        }

        // Write the simulation output string to the output file
        const size_t stringlen = strlen(result.output);
        if (fwrite(result.output, 1, stringlen, file) != stringlen) {
            fprintf(stderr, "Error writing output to terminal file\n");
            free(result.output);
            return 1;
        }

        fclose(file);
        printf("Terminal output written successfully.\n");
    }

    // Print final execution summary to the console
    printf("\n========== EXECUTION SUMMARY ===========\n");
    printf("Total Cycles Executed      : %u\n", result.cycles);
    printf("Number of terminal outputs : %u\n", result.outs);
    printf("Number of terminal inputs  : %u\n", result.ins);
    printf("========================================\n");

    free(result.output);
    free(instructions);
    return 0;
}
