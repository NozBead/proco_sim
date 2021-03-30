CC = gcc
CFLAGS = -g -Wall -Wextra

vpath %.c src/
vpath %.h src/

proco_sim : micro_instruction.o disassembly.o instruction.o debug.o main.o
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -rf *.o
