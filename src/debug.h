#ifndef __H_DEBUG__
#define __H_DEBUG__

#define BRKPTS_MAX 10

void print_registers();
void handle_break(int *breaked, int instr_idx, int *brkpts, int brkpts_size);

#endif
