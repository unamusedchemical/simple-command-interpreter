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
        close(fd);
        errx(1, "Problem with read()");
    }

    if (0 == count) {
        return NULL;
    }

    char* line = malloc(sizeof(char) * (count + 1));
    if (line == NULL) {
        close(fd);
        errx(20, "Problem with malloc()");
    }


    if (-1 == lseek(fd, -(count+1), SEEK_CUR)) {
        close(fd);
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
    if (-1 == readStatus ) {
        close(fd);
        free(line);
        errx(1, "Problem with read()");
    }

    return line;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        errx(40, "No arguments provided!");
    } else if (argc != 2) {
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
        // if the tokenizer fails, it returns null
        if (tokens == NULL) {
            close(fd);
            errx(20, "Tokenisation failed!");
        }

        pid_t childPid = fork();
        if (-1 == childPid) {
            freeTokens(tokens);
            close(fd);
            errx(30, "Problem with fork()");
        }
        if (0 == childPid) {
            if (-1 == execvp(tokens[0], tokens)) {
                freeTokens(tokens);
                close(fd);
                errx(31, "Problem with execvp()");
            }
        } else {
            int status;
            if (-1 == wait(&status)) {
                freeTokens(tokens);
                close(fd);
                errx(32, "Problem with wait()");
            }

            if (WIFEXITED(status)) {
                printf("Child exited with status %d\n", WEXITSTATUS(status));
            } else {
                printf("Child exited abnormally");
            }
        }
        freeTokens(tokens);
    }
    close(fd);

    return 0;
}