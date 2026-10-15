# Should print "CPU!" to the terminal
# 8000 cycles recommended

li x1, 110
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

li x1, 110
jal x31, print
