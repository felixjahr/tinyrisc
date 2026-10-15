# Includes print and read function and an optimized print function
# print: Prints a null-terminated string from main memory address passed in x1 to terminal
# read: Reads user input from terminal and stores it in main memory address passed in x1 to terminal

jal x0, stdio_end


# Defining print function 
print:
    # Loading address in x5 register
    add x5, x1, x0
    # Loading terminal address in x6 register
    not x6, x0
    li x28, 0xff

# Looping until null terminator is found
print_loop:
    # Loading next byte in x7 register
    ld x7, x5
    # Giving the byte to the terminal
    sd x7, x6
    # Applying bitmask
    and x7, x7, x28
    # If byte is null terminator jump to end
    beq x7, x0, print_end
    # Move to the next byte by incrementing the pointer
    inc x5, x5
    # Jump to start of loop (made possible due to implementation of negative offsets)
    jal x0, print_loop

print_end:
    j x31



# Defining read function
read:
    # Loading adress in x5 register
    add x5, x1, x0
    # Loading terminal address in x6 register
    not x6, x0

# Looping until null terminator is found
read_loop:
    # Get next byte from terminal in x7 register
    ld x7, x6
    # Storing the byte in main memory
    sd x7, x5
    # If byte is null terminator jump to end
    beq x7, x0, read_end
    # Move to the next byte by incrementing the pointer
    inc x5, x5
    # Jump to start of loop (made possible due to implementation of negative offsets)
    jal x0, read_loop

read_end:
    j x31



# Optimizes the number of ld instruction needed
print_optimized:
    # Loading address in x5 register
    add x5, x1, x0
    # Loading terminal address in x6 register
    not x6, x0
    # Load 
    li x27, 4
    # Loading bitmask
    li x28, 0xff
    # Loading value for Shifting
    li x29, 8
    
# Looping until null terminator is found
print_optimized_loop:
    # Loading next byte in x7 register
    ld x7, x5
    # Giving the byte to the terminal
    sd x7, x6
    # Applying bitmask
    add x30, x7, x0
    and x30, x30, x28
    # If byte is null terminator jump to end
    beq x30, x0, print_optimized_end

    # Used loop unrolling to save instructions
    # Shifting right to get next byte
    srl x7, x7, x29
    # Giving the byte to the terminal
    sd x7, x6
    # Applying bitmask
    add x30, x7, x0
    and x30, x30, x28
    # If byte is null terminator jump to end
    beq x30, x0, print_optimized_end

    # Shifting right to get next byte
    srl x7, x7, x29
    # Giving the byte to the terminal
    sd x7, x6
    # Applying bitmask
    add x30, x7, x0
    and x30, x30, x28
    # If byte is null terminator jump to end
    beq x30, x0, print_optimized_end

    # Shifting right to get next byte
    srl x7, x7, x29
    # Giving the byte to the terminal
    sd x7, x6
    # Applying bitmask
    add x30, x7, x0
    and x30, x30, x28
    # If byte is null terminator jump to end
    beq x30, x0, print_optimized_end

    # Move to the next 4 byte by incrementing the pointer
    add x5, x5, x27
    # Jump to start of loop (made possible due to implementation of negative offsets)
    jal x0, print_optimized_loop

print_optimized_end:
    j x31
    

stdio_end: