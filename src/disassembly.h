#ifndef __H_DISASS__
#define __H_DISASS__

void str_jmp(char *dest, unsigned char *args, unsigned char *mem_args);
void str_jz(char *dest, unsigned char *args, unsigned char *mem_args);
void str_jc(char *dest, unsigned char *args, unsigned char *mem_args);
void str_jmprx(char *dest, unsigned char *args, unsigned char *mem_args);
void str_strx(char *dest, unsigned char *args, unsigned char *mem_args);
void str_ldrx(char *dest, unsigned char *args, unsigned char *mem_args);
void str_st(char *dest, unsigned char *args, unsigned char *mem_args);
void str_ld(char *dest, unsigned char *args, unsigned char *mem_args);
void str_mv(char *dest, unsigned char *args, unsigned char *mem_args);
void str_dec(char *dest, unsigned char *args, unsigned char *mem_args);
void str_inc(char *dest, unsigned char *args, unsigned char *mem_args);
void str_not(char *dest, unsigned char *args, unsigned char *mem_args);
void str_add(char *dest, unsigned char *args, unsigned char *mem_args);
void str_sub(char *dest, unsigned char *args, unsigned char *mem_args);
void str_and(char *dest, unsigned char *args, unsigned char *mem_args);
void str_swp(char *dest, unsigned char *args, unsigned char *mem_args);
void str_mvreg(char *dest, unsigned char *args, unsigned char *mem_args);

#endif
