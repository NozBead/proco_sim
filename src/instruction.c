#include "micro_instruction.h"
#include "disassembly.h"
#include "instruction.h"

void incr_pc() {
	PCout();
	ALin();
	AAout();
	PCin();
}

void read_pc() {
	PCout();
	ALin();
	Read();
}

void pc_to_ir() {
	read_pc();
	DLout();
	IRin();
}

void JMP() {
	// first argument (PC low) to PC low
	read_pc();
	DLout();
	PCLin();
	// second argument (PC high) to PC high
	AAout();
	ALin();
	Read();
	DLout();
	PCHin();
}

void JZ() {
	if (*alu_flags & ZMSK) {
		JMP();
	}
	else {
		incr_pc();
		incr_pc();
	}
}

void JC() {
	if (*alu_flags & CMSK) {
		JMP();
	}
	else {
		incr_pc();
		incr_pc();
	}
}

void JMPRX() {
	SR(1);
	Rout();
	PCLin();
	SR(2);
	Rout();
	PCHin();
}

void reg_to_mem(unsigned char n) {
	SR(n);
	Rout();
	DLin();
	Write();
}

void mem_to_reg(unsigned char n) {
	Read();
	DLout();
	SR(n);
	Rin();
}

void rx_to_addr(unsigned char n) {
	SR(++n);
	Rout();
	ALLin();
	SR(++n);
	Rout();
	ALHin();
}

void STRX(unsigned char *args) {
	rx_to_addr(args[0]);
	reg_to_mem(0);
}

void LDRX(unsigned char *args) {
	rx_to_addr(args[0]);
	mem_to_reg(0);
}

void params_to_addr() {
	// first param (AL low) to data bus
	read_pc();
	DLout();
	// second param (AL high) to DL
	incr_pc();
	PCout();
	ALin();
	Read();
	// first param from data bus to AL low
	ALLin();
	// second param to AL high
	DLout();
	ALHin();
}

void ST(unsigned char *args) {
	params_to_addr();
	reg_to_mem(args[0]);
	incr_pc();
}

void LD(unsigned char *args) {
	params_to_addr();
	mem_to_reg(args[0]);
	incr_pc();
}

void MV(unsigned char *args) {
	// first param (value) to DL
	read_pc();
	// DL to register
	DLout();
	SR(args[0]);
	Rin();
	incr_pc();
}

void reg_to_x(unsigned char n) {
	SR(n);
	Rout();
	Xin();
}

void reg_to_y(unsigned char n) {
	SR(n);
	Rout();
	Yin();
}

void alu_to_reg(unsigned char n) {
	ALUout();
	SR(n);
	Rin();
}


void alu_arg_op(unsigned char n, void (*alu_op)(void)) {
	reg_to_x(n);
	alu_op();
	alu_to_reg(n);
}

void alu_args_op(unsigned char n, unsigned char m, void (*alu_op)(void)) {
	reg_to_x(m);
	reg_to_y(n);
	alu_op();
	alu_to_reg(n);
}

void ADD(unsigned char *args) {
	alu_args_op(args[0], args[1], &alu_add);
}

void SUB(unsigned char *args) {
	alu_args_op(args[0], args[1], &alu_sub);
}

void AND(unsigned char *args) {
	alu_args_op(args[0], args[1], &alu_and);
}

void DEC(unsigned char *args) {
	alu_arg_op(args[0], &alu_dec);
}

void INC(unsigned char *args) {
	alu_arg_op(args[0], &alu_inc);
}

void NOT(unsigned char *args) {
	alu_arg_op(args[0], &alu_not);
}

void SWP(unsigned char *args) {
	// Rm in DL
	SR(args[1]);
	Rout();
	DLin();
	// Rn to Rm
	SR(args[0]);
	Rout();
	SR(args[1]);
	Rin();
	// DL to Rn
	DLout();
	SR(args[0]);
	Rin();
}

void MVReg(unsigned char *args) {
	// Rm to Rn
	SR(args[1]);
	Rout();
	SR(args[0]);
	Rin();
}

instr instrs[] = {
	{&JMP,		&str_jmp,	0b01110000,	0,	{},	2},
	{&JZ,		&str_jz,	0b01110001,	0,	{},	2},
	{&JC,		&str_jc,	0b01110010,	0,	{},	2},
	{&JMPRX,	&str_jmprx,	0b01110011,	0,	{},	0},
	{&STRX, 	&str_strx,	0b01111000,	1,	{2},	0},
	{&LDRX, 	&str_ldrx,	0b01111100,	1,	{2},	0},
	{&ST,		&str_st,	0b01000000,	1,	{3},	2},
	{&LD,		&str_ld,	0b01001000,	1,	{3},	2},
	{&MV,		&str_mv,	0b01010000,	1,	{3},	1},
	{&DEC,		&str_dec,	0b01011000,	1,	{3},	0},
	{&INC,		&str_inc,	0b01100000,	1,	{3},	0},
	{&NOT,		&str_not,	0b01101000,	1,	{3},	0},
	{&ADD,		&str_add,	0b10000000,	2,	{2, 3},	0},
	{&SUB,		&str_sub,	0b10100000,	2,	{2, 3},	0},
	{&AND,		&str_and,	0b11000000,	2,	{2, 3},	0},
	{&SWP,		&str_swp,	0b11100000,	2,	{2, 3},	0},
	{&MVReg,	&str_mvreg,	0b00000000,	2,	{3, 3},	0}
};

void eliminate(decoder *dec, unsigned char curr_bit, int offset) {
	for (int j = 0 ; j < NB_INSTR ; j++) {
		instr *instr = instrs+j;

		if (!dec->bad[j]) {
			unsigned char bit = instr->code >> offset;
			if (curr_bit != bit) {
				dec->bad[j] = 1;
				dec->nb_ok--;
			}
			else {
				dec->last_ok = instr;
			}
		}
	}
}

void init_decoder(decoder *dec) {
	dec->nb_ok = NB_INSTR;
	for (int i = 0 ; i < NB_INSTR ; i++) {
		dec->bad[i] = 0;
	}
}

instr *decode(unsigned char code) {
	decoder dec;
	init_decoder(&dec);

	// For each bit
	for (int i = 7 ; i >= 0 && dec.nb_ok > 1 ; i--) {
		unsigned char curr_bit = code >> i;
		eliminate(&dec, curr_bit, i);
	}

	return dec.last_ok;
}
