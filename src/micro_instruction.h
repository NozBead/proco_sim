#ifndef __H_MICR__
#define __H_MICR__

#define MEM_SIZE 1024
#define REG_NB 8

typedef struct {
	unsigned char regs[REG_NB];
	short curr_reg;
} registers_t;

#ifndef MICR_NO_EXT_DEF
extern unsigned char ir_reg[1];
extern unsigned char pc_reg[2];
extern unsigned char *memory;
extern unsigned char alu_flags[1];
extern registers_t registers;
#endif

void IRin();
void Xin();
void Yin();
void DLin();
void DLout();
void PCin();
void PCLin();
void PCHin();
void PCout();
void ALin();
void ALLin();
void ALHin();
void AAout();
void SR(short n);
void Rin();
void Rout();

void Write();
void Read();

int pc_value();

#define ZMSK 1 
#define CMSK 1 << 1
void ALUout();
void alu_add();
void alu_sub();
void alu_and();
void alu_dec();
void alu_inc();
void alu_not();
#endif
