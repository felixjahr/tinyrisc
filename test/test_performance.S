# Used for testing performance depending on character count

add x1, x0, x0
add x2, x0, x0
li x3, 1

loop:
    li x4, 71
    sd x4, x2

    inc x2, x2

    bne x2, x3, loop


jal x31, print