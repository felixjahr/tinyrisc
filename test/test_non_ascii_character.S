# Tests what happens when printing ascii character thats out of bounds
# 4000 cycles recommended

li x1, 100
li x10, 250
sd x10, x1
jal x31, print