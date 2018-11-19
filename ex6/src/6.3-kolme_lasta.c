#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>

int pipe1_fd[2], pipe2_fd[2], pipe3_fd[3];
int new_pid;

int main() {
    if(pipe(pipe1_fd) < 0 || pipe(pipe2_fd) < 0 || pipe(pipe3_fd) < 0) {
        perror("Error creating pipe\n");
    }
    
    new_pid = fork();

    if(new_pid < 0) {
        perror("Error in fork(): ");
        exit(EXIT_FAILURE);
    }

    
    



}