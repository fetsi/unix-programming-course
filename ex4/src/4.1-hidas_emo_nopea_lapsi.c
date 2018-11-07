#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/*Kun taustalle käynnistettyä ohjelmaa katselee ps -l -komennolla, huomataan, että työnsä loppuun suorittanut lapsiprosessi on tilassa "defunct" eli ns. zombina.
Sillä ei enää ole koodia suoritettavana, mutta se on silti vielä olemassa prosessitaulussa, koska äitiprosessi ei ole ehtinyt kuitata sitä wait() -funktiolla.
pstree -u [käyttäjätunnus] -komennolla nähdään, että äitiprosessin äiti on bash, eli käytössä oleva komentotulkki.*/

int main() {

    pid_t new_pid = fork();
    if(new_pid == -1) {
        perror("fork() failed: ");
        exit(EXIT_FAILURE);
    }

    if(new_pid == 0) {
        //This is the child process
        printf("Hello, this is the child. My PID is %d. My parent's PID is %d\n", getpid(), getppid());
        return 0;
    }

    else {
        printf("Hello, this is the parent. My PID is %d. My child's PID is %d\n", getpid(), new_pid);
        sleep(20);
        wait(NULL);
        printf("Exiting...\n");
        return 0;
    }
}