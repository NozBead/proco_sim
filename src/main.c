#include "micro_instruction.h"
#include "instruction.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void get_args(unsigned char *dest, unsigned char raw_instr, instr *instr) {
	unsigned char offset = 0;
	unsigned char size = 0;
	unsigned char mask = 0;
	for (int i = instr->nb_args-1 ; i >= 0 ; i--) {
		size = instr->size_args[i];
		mask = (1 << size) - 1;
		dest[i] = (raw_instr >> offset) & mask;
		offset += size;
	}
}

void get_mem_args(unsigned char *dest, unsigned char *curr_mem, instr *instr) {
	for (int i = 0 ; i < instr->nb_mem_args ; i++) {
		dest[i] = curr_mem[i+1];
	}
}

void ctrl_unit_loop(int *brkpts, int brkpts_size) {
	char instr_name[16];
	unsigned char args[ARGS_MAX];
	unsigned char mem_args[ARGS_MAX];

	int prog_end = 0;
	int breaked = 0;
	int pc = 0;
	while (!prog_end) {
		pc = pc_value();

		pc_to_ir();
		instr *instr = decode(*ir_reg);

		prog_end = (*ir_reg == 0xFF) && (memory[pc+1] == 0xFF);
		if (!prog_end) {
			get_args(args, *ir_reg, instr);
			get_mem_args(mem_args, memory+pc, instr);

			instr->tostr(instr_name, args, mem_args);
			printf("%04X %s\n", pc, instr_name);
			handle_break(&breaked, pc, brkpts, brkpts_size);

			incr_pc();
			instr->exec(args);
		}
	}
}

int load_prog(unsigned char *mem, const char *pathname, int size) {
 	int fd = open(pathname, O_RDONLY);
	if (fd == -1) {
		perror(pathname);
		return -1;
	}

	int mem_off = read(fd, mem, size);
	if (mem_off == -1) {
		perror("Error reading");
		return -1;
	}

	return mem_off;
}

int load_brkpts(int *brkpts, char **argv, int argc, int offset) {
	int brkpts_size = 0;
	for (int i = offset ; i < argc ; i++) {
		brkpts[brkpts_size] = atoi(argv[i]);
		brkpts_size++;
	}
	return brkpts_size;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage : %s hex_program [breakpoint...]\n", argv[0]);
		return 1;
	}

	memory = malloc(MEM_SIZE);
	if (load_prog(memory, argv[1], MEM_SIZE) == -1) {
		return 2;
	}

	int brkpts[BRKPTS_MAX];
	int brkpts_size = load_brkpts(brkpts, argv, argc, 2);

	printf("### Program start ###\n\n");
	ctrl_unit_loop(brkpts, brkpts_size);
	printf("\n### Program end ###\n");
	print_registers();

	free(memory);
	return 0;
}
