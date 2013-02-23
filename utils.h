#ifndef UTILS
#define UTILS

int number_of_bytes(const instr* i);
int stricmp (const char *p1, const char *p2);
char* strtoupper(const char* src);
instr* create_instr(const int opcode, const int operand);

int among(const char c, const char* str);
int is_blank(const char c);
const char* skip_blank(const char* c);

#endif
