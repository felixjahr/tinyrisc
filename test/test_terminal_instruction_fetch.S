# Tests the behavior when pc reaches address which is memory mapped to terminal
# Assumes that terminal memory mapping would be set to 100 and pc starts at 0 then inc will be called every time the user enters c because c in ASCII is 0x01100011 which translates to j x0 instruction

inc x1, x1

li x2, 100
j x2