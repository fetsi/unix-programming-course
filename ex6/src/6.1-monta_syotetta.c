#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_LINESIZE 256


void sig_handler(int sig) {
    char buf[MAX_LINESIZE] = "In handler, exiting program\n";
    write(STDOUT_FILENO, (void*)buf, MAX_LINESIZE);
    exit(EXIT_SUCCESS);
}

void child_loop(int pipe_write_fd) {
    char buf[MAX_LINESIZE];
    printf("In child, PID: %d\n", getpid());

    for(int i = 0; i < 20; i++) {
        sprintf(buf, "Child doing iteration %d", i);
        write(pipe_write_fd, (void*)buf, strlen(buf));
        sleep(1);
    }    
    printf("Exiting child\n");
    exit(EXIT_SUCCESS);
}

void parent_loop(int pipe_read_fd) {
    while(1) {
        char rbuf_stdin[MAX_LINESIZE + 1];
        char rbuf_pipe[MAX_LINESIZE + 1];
        int rcv_stdin, rcv_pipe;

        rcv_stdin = read(STDIN_FILENO, (void*)rbuf_stdin, MAX_LINESIZE);

        if(rcv_stdin == -1) {
            if(errno != EAGAIN) {
                perror("Error reading from stdin: ");
                exit(EXIT_FAILURE);
            }
            //The error was EAGAIN, so we will just wait for some input
        }

        rcv_pipe = read(pipe_read_fd, (void*)rbuf_pipe, MAX_LINESIZE);

        if(rcv_pipe == -1) {
            if(errno != EAGAIN) {
                perror("Error reading from pipe: ");
                exit(EXIT_FAILURE);
            }
            //The error was EAGAIN, so we will just wait for some input
        }

        if(rcv_stdin > 0) {
            printf("%s\n", rbuf_stdin);
        }
        if(rcv_pipe > 0) {
            printf("%s\n", rbuf_pipe);
        }
    }
}


int main() {
    int pipe_fd[2];
    struct sigaction action;

    //Set handler function for the signal and register the handler
    action.sa_handler = sig_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGCHLD, &action, NULL);

    printf("In parent, PID: %d\n", getpid());

    if(pipe(pipe_fd) < 0) {
        perror("Failed to create pipe: ");
        exit(EXIT_FAILURE);
    }

    int pipe_flags = fcntl(pipe_fd[0], F_GETFL, 0);
    int stdin_flags = fcntl(STDIN_FILENO, F_GETFL, 0);

    if(fcntl(pipe_fd[0], F_SETFL, pipe_flags | O_NONBLOCK) < 0 || fcntl(STDIN_FILENO, F_SETFL, stdin_flags | O_NONBLOCK) < 0) {
        perror("Error when trying to set file descriptors to O_NONBLOCK: ");
        exit(EXIT_FAILURE);
    }

    pid_t new_pid = fork();

    if(new_pid < 0) {
        perror("fork() failed: ");
        exit(EXIT_FAILURE);
    }

    if(new_pid == 0) {
        //Child continues from here
        child_loop(pipe_fd[1]);
    } 
    else {
        //Parent continues from here
        parent_loop(pipe_fd[0]);
    }
}