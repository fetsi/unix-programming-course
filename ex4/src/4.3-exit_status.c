#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main() {
    //Otetaan heti alussa talteen äidin PID
    pid_t parent_pid = getpid();

    pid_t new_pid1 = fork();
    if(new_pid1 == -1) {
        perror("fork() failed\n");
        exit(-10);
    }
    if(new_pid1 == 0) {
        //Ollaan ensimmäisessä lapsiprosessissa
        printf("Eka lapsi valmis\n");
        exit(1);
    }

    //Äiti jatkaa tästä
    pid_t new_pid2 = fork();

    if(new_pid2 == -1) {
        perror("fork() failed\n");
        exit(-10);
    }

    if(new_pid2 == 0) {
        //Ollaan toisessa lapsessa
        sleep(3);
        printf("Toka lapsi valmis");
        exit(2);
    }

    


}