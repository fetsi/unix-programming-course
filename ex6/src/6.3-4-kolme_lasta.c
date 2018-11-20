#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>

int pipe1_fd[2], pipe2_fd[2];
int new_pid;
char buf[256];

void exec_ps();
void exec_grep();
void exec_wc();


int main() {
    if(pipe(pipe1_fd) < 0 || pipe(pipe2_fd) < 0) {
        perror("Error creating pipe\n");
    }

    new_pid = fork();

    if(new_pid == 0) {
        //In first child, execute the ps command
        exec_ps();
    }
    
    new_pid = fork();

    if(new_pid == 0) {
        //In the second child, execute grep
        exec_grep();
    }

    new_pid = fork();

    if(new_pid == 0) {
        //In the third child, execute wc
        exec_wc();
    }
    else {
        //Close the file descriptors that we don't need
        close(pipe1_fd[0]);
        close(pipe1_fd[1]);
        close(pipe2_fd[1]);
        exit(EXIT_SUCCESS);
    }
    
}

void exec_ps() {
    //Close descriptors that we don't need
    close(pipe1_fd[0]);
    close(pipe2_fd[0]);
    close(pipe2_fd[1]);

    if(dup2(pipe1_fd[1], STDOUT_FILENO) < 0) {
        perror("Error duplicating file descriptor in exec_ps(): ");
        exit(EXIT_FAILURE);
    }

    if(execlp("ps", "ps", "-A", (char*)NULL) == -1) {
        //Shouldn't ever reach this point unless changing the process image failed
        perror("Error executing \"ps -A\": ");
        exit(EXIT_FAILURE);
    }
}

void exec_grep() {
    close(pipe1_fd[1]);
    close(pipe2_fd[0]);

    if(dup2(pipe1_fd[0], STDIN_FILENO) < 0 || dup2(pipe2_fd[1], STDOUT_FILENO) < 0) {
        perror("Error duplicating file descriptor in exec_grep(): ");
        exit(EXIT_FAILURE);
    }
    if(execlp("grep", "grep", "bash", (char*)NULL) == -1) {
        perror("Error executing \"grep bash\": ");
        exit(EXIT_FAILURE);
    }
}

void exec_wc() {
    //Close the file descriptors that we don't need
    close(pipe1_fd[0]);
    close(pipe1_fd[1]);
    close(pipe2_fd[1]);

    if(dup2(pipe2_fd[0], STDIN_FILENO) < 0) {
        perror("Error duplicating file descriptor in exec_wc(): ");
        exit(EXIT_FAILURE);
    }

    if(execlp("wc", "wc", "-l", (char*)NULL) == -1) {
        perror("Error executing \"wc -l\": ");
        exit(EXIT_FAILURE);
    }
}