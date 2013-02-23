#ifndef INSTR
#define INSTR

opcode* get_opcode(const char* mnemo);
void init_opcodes();
void destroy_opcodes();

#endif
