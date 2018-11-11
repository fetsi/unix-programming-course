#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int child_ex_status;
int pipe_fd[2];
const char *msg = "This is a line\nThis is another line\nAnd a third one";

int main() {

    if(pipe(pipe_fd) != 0) {
        perror("Error creating pipe: ");
        exit(EXIT_FAILURE);
    }

    pid_t new_pid = fork();
    if(new_pid == -1) {
        perror("fork() failed: ");
        exit(EXIT_FAILURE);
    }
    if(new_pid == 0) {
        //In the child      
        close(pipe_fd[0]); //We don't need the reading end of the pipe, so close it
        int wr = write(pipe_fd[1], (const void*)msg, strlen(msg) );
        if(wr == -1) {
            perror("Error writing to pipe from child: ");
            exit(EXIT_FAILURE);
        }
        printf("%d bytes written to pipe by child process\n", wr);
        close(pipe_fd[1]); //We are done here, so close the writing end too                            
        exit(EXIT_SUCCESS);
    }

    //Parent continues from here
    if(wait(&child_ex_status) == -1) {
        perror("Error waiting on child: ");
        exit(EXIT_FAILURE);
    }

    if(WIFEXITED(child_ex_status)) {
        printf("Child process exited with status %d\n", WEXITSTATUS(child_ex_status));
    }

    char *read_buf = calloc(strlen(msg) + 1, 1); //Allocate heap memory for the incoming message string and one extra byte for the terminating '\0'
    if(read_buf == NULL) {
        fprintf(stderr, "calloc() failed\n");
    }
    close(pipe_fd[1]); //We don't need the writing end of the pipe

    //Read contents
    int rd = read(pipe_fd[0], read_buf, strlen(msg));
    if(rd < 0) {
        perror("Failed to read pipe from parent process\n");
        exit(EXIT_FAILURE);
    }
 
    printf("%d bytes read from pipe by parent process\n", rd);
    printf("%s\n", read_buf);
    close(pipe_fd[0]);

    return 0;
}