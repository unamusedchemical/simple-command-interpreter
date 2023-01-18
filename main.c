#include "tokenizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>


char* readline(int fd) {
    int count = 0;
    ssize_t readStatus;
    char temp;

    while (1 == (readStatus = read(fd, &temp, 1))) {
        count++;
        if (temp == '\n') {
            break;
        }
    }
    if (-1 == readStatus) {
        errx(1, "Problem with read()");
    }

    char* line = malloc(sizeof(char) * count);

    if (-1 == lseek(fd, SEEK_CUR, -count)) {
        free(line);
        errx(3, "Problem with lseek()");
    }

    int index = 0;
    while(1 == (readStatus = read(fd, &temp, 1))) {
        if (temp == '\n') {
            line[index] = '\n';
            break;
        }
        line[index++] = temp;
    }

    return line;
}

int main() {

    int fd = open("pesho", O_RDONLY);

    char* line;
    while (NULL != (line = readline(fd))) {
        char** tokens = tokenizer(line);
        free(line);
        if (tokens == NULL) {
            errx(23, "Invalid data to tokenize!");
        }


        freeTokens(tokens);
    }

    return 0;
}

