#define MICR_NO_EXT_DEF
#include "micro_instruction.h"

unsigned char *memory;
unsigned char ir_reg[1];

registers_t registers;
unsigned char pc_reg[] = {0, 0};
unsigned char addr_reg[2];
unsigned char data_reg[1];

unsigned char data_bus[1];
unsigned char addr_bus[2];

unsigned char x_reg[1];
unsigned char y_reg[1];
unsigned char alu_reg[1];
int alu_reg_val;
unsigned char alu_flags = 0;

void move_data(unsigned char *src, unsigned char *dest, short size) {
	for (short i = 0 ; i < size ; i++) {
		dest[i] = src[i];
	}
}

void reg_out(unsigned char *reg, unsigned char *bus, short size) {
	move_data(reg, bus, size);
}

void reg_in(unsigned char *reg, unsigned char *bus, short size) {
	move_data(bus, reg, size);
}

void IRin() {
	reg_in(ir_reg, data_bus, 1);
}

void Xin() {
	reg_in(x_reg, data_bus, 1);
}

void Yin() {
	reg_in(y_reg, data_bus, 1);
}

void DLin() {
	reg_in(data_reg, data_bus, 1);
}

void DLout() {
	reg_out(data_reg, data_bus, 1);
}

void PCin() {
	reg_in(pc_reg, addr_bus, 2);
}

void PCLin() {
	reg_in(pc_reg + 1, data_bus, 1);
}

void PCHin() {
	reg_in(pc_reg, data_bus, 1);
}

void PCout() {
	reg_out(pc_reg, addr_bus, 2);
}

void ALin() {
	reg_in(addr_reg, addr_bus, 2);
}

void ALLin() {
	reg_in(addr_reg + 1, data_bus, 1);
}

void ALHin() {
	reg_in(addr_reg, data_bus, 1);
}

void AAout() {
	unsigned char tmp[2];
	move_data(addr_reg, tmp, 2);

	tmp[1]++;

	if (tmp[1] == 0) {
		tmp[0]++;
	}

	reg_out(tmp, addr_bus, 2);
}

void SR(short n) {
	registers.curr_reg = n;
}

void Rout() {
	reg_out(registers.regs + registers.curr_reg, data_bus, 1);
}

void Rin() {
	reg_in(registers.regs + registers.curr_reg, data_bus, 1);
}

unsigned char *addr_value() {
	return memory + ((addr_reg[0] << 8) | addr_reg[1]);
}

int pc_value() {
	return pc_reg[0] << 8 | pc_reg[1];
}

void Write() {
	reg_out(data_reg, addr_value(), 1);
}

void Read() {
	reg_in(data_reg, addr_value(), 1);
}

void ALUout() {
	reg_out(alu_reg, data_bus, 1);
}

void set_alu() {
	*alu_reg = (unsigned char) alu_reg_val;
	alu_flags = 0;

	if (*alu_reg == 0) {
		alu_flags |= ZMSK;
	}
	if (alu_reg_val > 255 || alu_reg_val < 0) {
		alu_flags |= CMSK;
	}
}

void alu_add() {
	alu_reg_val = *x_reg + *y_reg;
	set_alu();
}

void alu_sub() {
	alu_reg_val = *x_reg + *y_reg;
	set_alu();
}

void alu_and() {
	alu_reg_val = *x_reg & *y_reg;
	set_alu();
}

void alu_dec() {
	alu_reg_val = *x_reg - 1;
	set_alu();
}

void alu_inc() {
	alu_reg_val = *x_reg + 1;
	set_alu();
}

void alu_not() {
	alu_reg_val = ~*x_reg;
	set_alu();
}
