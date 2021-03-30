#include <stdio.h>

void str_j(char *dest, const char *name, unsigned char *mem_args) {
	sprintf(dest, "%s %X%Xh",
			name, mem_args[1], mem_args[0]);
}

void str_jmp(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_j(dest, "JMP", mem_args);
}

void str_jz(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_j(dest, "JZ", mem_args);
}

void str_jc(char *dest, unsigned char *args, unsigned char *mem_args) {

	str_j(dest, "JC", mem_args);
}

void str_jmprx(char *dest, unsigned char *args, unsigned char *mem_args) {
	sprintf(dest, "JMP RX0");
}

void str_memrx(char *dest, const char *name, unsigned char *args) {
	sprintf(dest, "%s R%d, RX%d",
			name, args[0], args[1]);
}

void str_strx(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_memrx(dest, "ST", args);
}

void str_ldrx(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_memrx(dest, "LD", args);
}

void str_mem(char *dest, const char *name, unsigned char *args, unsigned char *mem_args) {
	sprintf(dest, "%s R%d, %X%Xh",
			name, args[0], mem_args[1], mem_args[0]);
}

void str_st(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_mem(dest, "ST", args, mem_args);
}

void str_ld(char *dest, unsigned char *args, unsigned char *mem_args) {

	str_mem(dest, "LD", args, mem_args);
}

void str_mv(char *dest, unsigned char *args, unsigned char *mem_args) {
	sprintf(dest, "MV R%d, %X#",
			args[0], mem_args[0]);
}

void str_alu_one(char *dest, const char *name, unsigned char *args) {
	sprintf(dest, "%s R%d",
			name, args[0]);
}

void str_dec(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_alu_one(dest, "DEC", args);
}

void str_inc(char *dest, unsigned char *args, unsigned char *mem_args) {

	str_alu_one(dest, "INC", args);
}

void str_not(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_alu_one(dest, "NOT", args);
}

void str_alu_two(char *dest, const char *name, unsigned char *args) {
	sprintf(dest, "%s R%d, R%d",
			name, args[0], args[1]);
}

void str_add(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_alu_two(dest, "ADD", args);
}

void str_sub(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_alu_two(dest, "SUB", args);
}

void str_and(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_alu_two(dest, "AND", args);
}

void str_swp(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_alu_two(dest, "SWP", args);
}

void str_mvreg(char *dest, unsigned char *args, unsigned char *mem_args) {
	str_alu_two(dest, "MV", args);
}
