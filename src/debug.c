#include "micro_instruction.h"
#include <stdio.h>

void print_registers() {
	int nline = REG_NB / 2;
	for (int i = 0 ; i < nline ; i++) {
		int ileft = i*2;
		int iright = ileft+1;
		printf("R%d = 0x%02X\tR%d = 0x%02X\n",
			ileft, registers.regs[ileft],
			iright, registers.regs[iright]);
	}
}

int is_brkpt(int instr_idx, int *brkpts, int brkpts_size) {
	for (int i = 0 ; i < brkpts_size ; i++) {
		if (brkpts[i] == instr_idx) {
			return 1;
		}
	}
	return 0;
}

// returns breaked status
int breaked_prompt() {
	char buffer[4];
	int stay_breaked = 0;
	int loop_prompt = 1;

	while (loop_prompt) {
		printf("breaked$ ");
		fgets(buffer, 3, stdin);
	
		switch (buffer[0]) {
			case 'c' :
				loop_prompt = 0;
			break;
		
			case 's' :
				stay_breaked = 1;
				loop_prompt = 0;
			break;

			case 'p' :
				print_registers();
			break;
		}
	}

	return stay_breaked;
}

// prompt and update break status
void handle_break(int *breaked, int instr_idx, int *brkpts, int brkpts_size) {
	*breaked |= is_brkpt(instr_idx, brkpts, brkpts_size);
	if (*breaked) {
		*breaked = breaked_prompt();
	}
}
