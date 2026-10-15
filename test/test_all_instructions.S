# This test combines all available instrucions to check if something broke
# Uncomment DEBUG statement in modules.cpp to print registers after execution
# 3000 cycles recommended

not x1, x0                      # 0xFFFFFFFF in x1
li x2, 0xFF                     # 0xFF in x2
sub x3, x1, x2                  # 0xFFFFFF00 in x3
inc x4, x3                      # 0xFFFFFF01 in x4
and x5, x2, x4                  # 1 in x5
or x6, x3, x5                   # 0xFFFFFF01 in x6

test_label_1:
    beq x4, x6, test_label_3    # Branch should be executed
    add x1, x0, x5              # Should never be executed

test_label_2:
    blt x5, x4, test_label_3    # Branch should not be executed
    bgt x3, x4, test_label_3    # Branch should not be executed
    li x8, 41                   # 41 in x8
    sd x8, x3                   # Store 41 at 0xFFFFFF00 in main memory
    ld x9, x3                   # Read 41 from 0xFFFFFF00 in main memory to x9
    srl x10, x2, x5             # 0x7F in x10
    j x31                       # Jump to return adress from jal

test_label_3:
    add x7, x2, x5              # 0x100 in x7
    bne x4, x6, test_label_3    # Branch should not be executed
    jal x31, test_label_2       # Jump and store return address in x31