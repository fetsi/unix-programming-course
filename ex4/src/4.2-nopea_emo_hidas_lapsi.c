#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main() {

    pid_t new_pid = fork();
    if(new_pid == -1) {
        perror("fork() failed: ");
        exit(EXIT_FAILURE);
    }

    if(new_pid == 0) {
        //This is the child process
        sleep(20);
        printf("Hello, this is the child. My PID is %d. My parent's PID is %d\n", getpid(), getppid());
        return 0;
    }

    else {
        printf("Hello, this is the parent. My PID is %d. My child's PID is %d\n", getpid(), new_pid);
        return 0;
    }
}