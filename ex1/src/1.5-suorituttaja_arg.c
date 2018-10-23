#include <unistd.h> /* POSIX API */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256
#define MAXTOKENS 100
//Kommmentti

int main(void) {
    char buf[MAXLINE];
    pid_t pid;
    int status;
    printf("%% "); /* tulosta kehoite: %%, jotta tulostuu % */
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        buf[strlen(buf) - 1] = '\0'; /* korvaa rivinvaihtomerkki */
        if ((pid = fork()) < 0) {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) { /* lapsiprosessi jatkaa tästä */


        char *tokens[MAXTOKENS];

        tokens[0] = strtok(buf, " ");
        if (tokens[0] == NULL) {
            continue;
        }

        int i = 0;

        while(tokens[i] != NULL && i < MAXTOKENS) {
            i++;
            tokens[i] = strtok(NULL, " ");
        }
            execvp(tokens[0], tokens);
            fprintf(stderr,"couldn't execute execv: %s - %s",
            buf, strerror(errno));
            exit(EXIT_FAILURE);
        }
        /* mammaprosessi jatkaa tästä */
        if ((pid = waitpid(pid, &status, 0)) < 0) {
            perror("waitpid error");
            exit(EXIT_FAILURE);
        }
        printf("%% ");
    }
    exit(EXIT_SUCCESS);
}