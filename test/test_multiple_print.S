# Should print "Try\nsmall\nCPU!" to the terminal
# 25000 cycles recommended

li x1, 100
li x10, 84
sd x10, x1

inc x1, x1
li x10, 114
sd x10, x1

inc x1, x1
li x10, 121
sd x10, x1


li x1, 150
li x10, 115
sd x10, x1

inc x1, x1
li x10, 109
sd x10, x1

inc x1, x1
li x10, 97
sd x10, x1

inc x1, x1
li x10, 108
sd x10, x1

inc x1, x1
li x10, 108
sd x10, x1


li x1, 200
li x10, 67
sd x10, x1

inc x1, x1
li x10, 80
sd x10, x1

inc x1, x1
li x10, 85
sd x10, x1

inc x1, x1
li x10, 33
sd x10, x1

li x1, 100
jal x31, print

li x1, 150
jal x31, print

li x1, 200
jal x31, print
