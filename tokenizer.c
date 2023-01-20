#include "tokenizer.h"
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>

uint32_t countTokens(char* line) {
    uint32_t count = 0;
    int len = strlen(line);
    for (int i = 0; i < len; ++i) {
        if (line[i] == ' ' && ((i + 1) < len && line[i + 1] != ' ')) {
            count++;
        }
    }

    return ++count;
}

void getTokenSizes(char* line, uint32_t* tokenSizes) {
    int tokenIndex = 0;
    int len = strlen(line);
    for (int i = 0; i < len; i++) {
        if (line[i] == ' ') {
            if ((i + 1) < len && line[i + 1] != ' ') {
                tokenIndex++;
            }
        } else {
            tokenSizes[tokenIndex]++;
        }
    }
}

// if something fails in this function, it will return NULL
// this is checked in main.c, and if it is NULL, the program exits with a code of 20
char** tokenizer(char* line) {
    if (line == NULL) {
        return NULL;
    }

    uint32_t count = countTokens(line);
    uint32_t* sizes = calloc(count, sizeof(int));
    if (sizes == NULL) {
        return NULL;
    }
    getTokenSizes(line, sizes);

    char** tokens = malloc(sizeof(char*) * (count + 1));
    if (tokens == NULL) {
        free(sizes);
        return NULL;
    }
    tokens[count] = NULL;
    for (int i = 0; i < count; i++) {
        tokens[i] = malloc(sizeof(char) * (sizes[i] + 1));
        if (tokens[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(tokens[j]);
            }
            free(tokens);
            free(sizes);
            return NULL;
        }
    }

    free(sizes);

    int tokenIndex = 0;
    int tokenCharIndex = 0;
    int len = strlen(line);
    for (int i = 0; i < len; ++i) {
        if (line[i] == ' ') {
            if ((i + 1) < len && line[i + 1] != ' ') {
                tokens[tokenIndex][tokenCharIndex] = '\0';
                tokenIndex++;
                tokenCharIndex = 0;
            }
        } else {
            tokens[tokenIndex][tokenCharIndex++] = line[i];
        }
    }
    tokens[tokenIndex][tokenCharIndex] = '\0';
    
    return tokens;
}

void freeTokens(char** tokens) {
    char** tokensCopy = tokens;
    while(*tokensCopy != NULL) {
        free(*tokensCopy);
        tokensCopy++;
    }
    free(tokens);
}

void printTokens(char** tokens) {
    char** tokensCopy = tokens;
    while(*tokensCopy != NULL) {
        printf("%s ", *tokensCopy);
        tokensCopy++;
    }
}