#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 2048
#define MAX_ARGS 128

void run_command(char** args) {
    pid_t pid = fork();

    if (pid == 0) {
        // child process
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(1);
        }
    } else if (pid > 0) {
        // parent process
        wait(NULL);
    } else {
        perror("fork");
    }
}

int main(int argc, char* argv[]) {
    char line[MAX_LINE_LENGTH];
    char* args[MAX_ARGS];

    while (1) {
        printf("defintely not an overly long prompt: ");
        fgets(line, MAX_LINE_LENGTH, stdin);
        line[strcspn(line, "\n")] = 0; // remove newline

        char* token = strtok(line, " ");
        int i = 0;
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL) {
            continue; // empty command
        }

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "expected location\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("chdir");
                }
            }
        } else if (strcmp(args[0], "exit") == 0) {
            exit(0);
        } else {
            run_command(args);
        }
    }

    return 0;
}
