#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int child1_exstatus, child2_exstatus;

int main() {

    pid_t new_pid1 = fork();
    if(new_pid1 == -1) {
        perror("fork() failed\n");
        exit(-10);
    }
    if(new_pid1 == 0) {
        //In the first child
        printf("First child ready\n");
        exit(1);
    }

    //Parent continues from here
    pid_t new_pid2 = fork();

    if(new_pid2 == -1) {
        perror("fork() failed\n");
        exit(-10);
    }

    if(new_pid2 == 0) {
        //In the second child
        sleep(3);
        printf("Second child ready\n");
        exit(2);
    }

    if(waitpid(new_pid2, &child2_exstatus, 0) == -1){
        perror("Error in waitpid() on second child: ");
        exit(-11);
    }

    if(WIFEXITED(child2_exstatus)) {
        printf("Second child exited with code %d\n", WEXITSTATUS(child2_exstatus));
    }

    if(waitpid(new_pid1, &child1_exstatus, 0) == -1) {
        perror("Error in waitpid() on first child: ");
        exit(-11);
    }

    if(WIFEXITED(child1_exstatus)) {
        printf("Second child exited with code %d\n", WEXITSTATUS(child1_exstatus));
    }

    return 0;
}