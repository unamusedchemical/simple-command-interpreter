#ifndef TOKENIZER
#define TOKENIZER

#include <stdint.h>

char** tokenizer(char*);
uint32_t countTokens(char*);
void getTokenSizes(char*, uint32_t*);
void freeTokens(char**);

#endif
