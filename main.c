#include "tokenizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <err.h>
#include <sys/wait.h>

char* readline(int fd) {
    int count = 0;
    ssize_t readStatus;
    char temp;

    while (1 == (readStatus = read(fd, &temp, 1))) {
        if (temp == '\n') {
            break;
        }
        count++;
    }
    if (-1 == readStatus) {
        errx(1, "Problem with read()");
    }

    if (0 == count) {
        return NULL;
    }

    char* line = malloc(sizeof(char) * (count + 1));
    if (line == NULL) {
        errx(20, "Problem with malloc()");
    }


    if (-1 == lseek(fd, -(count+1), SEEK_CUR)) {
        free(line);
        errx(3, "Problem with lseek()");
    }

    int index = 0;
    while(1 == (readStatus = read(fd, &temp, 1))) {
        if (temp == '\n') {
            line[index] = '\0';
            break;
        }
        line[index++] = temp;
    }

    return line;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        errx(40, "No arguments provided!");
    }

    if (argc != 2) {
        errx(41, "Too many arguments provided!");
    }

    int fd = open(argv[1], O_RDONLY);
    if (-1 == fd) {
        errx(1, "Problem with open()");
    }

    char* line;
    while (NULL != (line = readline(fd))) {
        char** tokens = tokenizer(line);
        free(line);
        if (tokens == NULL) {
            errx(23, "Invalid data to tokenize!");
        }

        pid_t childPid = fork();
        if (-1 == childPid) {
            errx(30, "Problem with fork()");
        }
        printf("%d", childPid);
        if (0 == childPid) {
            if (-1 == execvp(tokens[0], tokens+1)) {
                errx(31, "Problem with execvp()");
            }
        } else {
            int status;
            if (-1 == wait(&status)) {
                errx(32, "Problem with wait()");
            }

            if (WIFEXITED(status)) {
                printf("Child exited with status %d", WEXITSTATUS(status));
            }
        }

        freeTokens(tokens);
    }

    return 0;
}