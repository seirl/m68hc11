#ifndef UTILS
#define UTILS

int number_of_bytes(const instr* i);
int stricmp (const char *p1, const char *p2);
char* strtoupper(const char* src);
instr* create_instr(const int opcode, const int operand);

int among(char c, char* str);
int is_blank(char c);
char* skip_blank(char* c);

#endif
