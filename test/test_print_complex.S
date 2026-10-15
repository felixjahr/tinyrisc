# Should print "Try small CPU!@#$%^&*()~\n{}[]\t|\\" to the console
# 50000 cycles recommended

li x1, 100
li x10, 84
sd x10, x1

inc x1, x1
li x10, 114
sd x10, x1

inc x1, x1
li x10, 121
sd x10, x1

inc x1, x1
li x10, 32
sd x10, x1

inc x1, x1
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

inc x1, x1
li x10, 32
sd x10, x1

inc x1, x1
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

# Testing unusual special characters
inc x1, x1
li x10, 64      # '@'
sd x10, x1

inc x1, x1
li x10, 35      # '#'
sd x10, x1

inc x1, x1
li x10, 36      # '$'
sd x10, x1

inc x1, x1
li x10, 37      # '%'
sd x10, x1

inc x1, x1
li x10, 94      # '^'
sd x10, x1

inc x1, x1
li x10, 38      # '&'
sd x10, x1

inc x1, x1
li x10, 42      # '*'
sd x10, x1

inc x1, x1
li x10, 40      # '('
sd x10, x1

inc x1, x1
li x10, 41      # ')'
sd x10, x1

inc x1, x1
li x10, 126     # '~'
sd x10, x1

inc x1, x1
li x10, 10      # '\n'
sd x10, x1

inc x1, x1
li x10, 123     # '{'
sd x10, x1

inc x1, x1
li x10, 125     # '}'
sd x10, x1

inc x1, x1
li x10, 91      # '['
sd x10, x1

inc x1, x1
li x10, 93      # ']'
sd x10, x1

inc x1, x1
li x10, 9      # '\t'
sd x10, x1

inc x1, x1
li x10, 124      # '|'
sd x10, x1


inc x1, x1
li x10, 92      # '\'
sd x10, x1


li x1, 100
jal x31, print
