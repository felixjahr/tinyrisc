# Should store 1 in x1, 2 in x2, 3 in x3, 4 in x4 and 5 in x5

inc x1, x0
jal x31, jump_1

jump_1:
    inc x2, x0
    add x2, x1, x2
    jal x31, jump_2

jump_4:
    inc x5, x0
    add x5, x4, x5
    jal x31, end

jump_3:
    inc x4, x0
    add x4, x4, x3
    jal x31, jump_4


jump_2:
    inc x3, x0
    add x3, x3, x2
    jal x31, jump_3

end:    