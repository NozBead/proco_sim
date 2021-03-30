#ifndef __H_INSTR__
#define __H_INSTR__

#define NB_INSTR 17
#define ARGS_MAX 2

typedef void (*instr_fct) (unsigned char *);
typedef void (*instr_str) (char *, unsigned char*, unsigned char*);

typedef struct {
	instr_fct exec;
	instr_str tostr;
	unsigned char code;
	unsigned char nb_args;
	unsigned char size_args[ARGS_MAX];
	unsigned char nb_mem_args;
} instr;

typedef struct {
	unsigned char bad[NB_INSTR];
	int nb_ok;
	instr *last_ok;
} decoder;

instr *decode(unsigned char code);
void read_pc();
void incr_pc();
void pc_to_ir();

#endif
