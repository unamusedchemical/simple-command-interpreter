#include "tokenizer.h"
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <stdio.h>

uint32_t countTokens(char* line) {
    uint32_t count = 0;
    for (int i = 0; i < strlen(line); ++i) {
        if (line[i] == ' ') {
            count++;
        }
    }

    return ++count;
}

void getTokenSizes(char* line, uint32_t* tokenSizes) {
    int tokenIndex = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ') {
            tokenIndex++;
        } else {
            tokenSizes[tokenIndex]++;
        }
    }
}

char** tokenizer(char* line) {
    if (line == NULL) {
        return NULL;
    }

    uint32_t count = countTokens(line);
    uint32_t* sizes = calloc(count, sizeof(int));
    if (sizes == NULL) {
        errx(21, "Error with calloc()");
    }
    getTokenSizes(line, sizes);

    char** tokens = malloc(sizeof(char*) * (count + 1));
    if (tokens == NULL) {
        errx(20, "Error with malloc()");
    }
    tokens[count] = NULL;
    for (int i = 0; i < count; i++) {
        tokens[i] = malloc(sizeof(char) * (sizes[i] + 1));
        if (tokens[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(tokens[j]);
            }
            free(tokens);
            errx(20, "Error with malloc()");
        }
    }

    int tokenIndex = 0;
    int tokenCharIndex;
    for (tokenCharIndex = 0; tokenCharIndex < strlen(line); ++tokenCharIndex) {
        if (line[tokenCharIndex] == ' ' || line[tokenCharIndex] == '\n') {
            tokens[tokenIndex][tokenCharIndex] = '\0';
            tokenIndex++;
        } else {
            tokens[tokenIndex][tokenCharIndex] = line[tokenCharIndex];
        }
    }
    tokens[tokenIndex][tokenCharIndex] = '\0';

    free(sizes);

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