#ifndef UTILS
#define UTILS

int number_of_bytes(const instr* i);
int stricmp (const char *p1, const char *p2);
char* strtoupper(const char* src);
statement* create_instr(const int opcode, const int operand);
statement* create_label(const char* label);

#endif
