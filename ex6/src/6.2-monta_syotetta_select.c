#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>

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

void read_input(int fd, char *rbuf) {
    
    int rcv = read(fd, (void*)rbuf, MAX_LINESIZE);
    if(rcv == -1) {
        perror("Error reading from stdin: ");
        exit(EXIT_FAILURE);
        }
    if(rcv > 0) {
        rbuf[rcv] = '\0'; //Add terminating NUL character
        printf("%s\n", rbuf);
    }

}



void parent_loop(int pipe_read_fd) {
    char rbuf_stdin[MAX_LINESIZE + 1];
    char rbuf_pipe[MAX_LINESIZE + 1];
    fd_set set;

    while(1) {
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(pipe_read_fd, &set);

        //The call to select will block until there is something to read from either stdin or the pipe
        if(select(pipe_read_fd + 1, &set, NULL, NULL, NULL) < 0) {
            perror("Error in select(): ");
            exit(EXIT_FAILURE);
        }

        if(FD_ISSET(STDIN_FILENO, &set)) {
            read_input(STDIN_FILENO, rbuf_stdin);
        }
        if(FD_ISSET(pipe_read_fd, &set)) {
            read_input(pipe_read_fd, rbuf_pipe);
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