CC = gcc
CXX = g++

SCPATH = $(SYSTEMC_HOME)

LDFLAGS = -L$(SCPATH)/lib -lsystemc -lm
CFLAGS = -I$(SCPATH)/include -Wall -Iinclude -Iinclude/risc -Iinclude/assembler 
CXXFLAGS = -std=c++14 -I$(SCPATH)/include -Wall -Iinclude -Iinclude/risc -Iinclude/assembler

CFLAGS += -g 
CXXFLAGS += -g

all: project assembler

main.o: src/risc/main.c
	$(CC) $(CFLAGS) -c $< -o $@

modules.o: src/risc/modules.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


project: main.o modules.o
	$(CXX) main.o modules.o -o project $(LDFLAGS)
	
ASSEMBLER_SRCS := $(wildcard src/assembler/*.c)

assembler: $(ASSEMBLER_SRCS)
	$(CC) $(CFLAGS) $^ -o assembler

clean:
	rm -f *.o project assembler
