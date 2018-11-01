#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 256

int new;
int fd;
char buf[MAX_LINE_LENGTH];
char *msg = "Anna kokonaisluku: ";

int main(int argc, char *argv[]) {

    if(argc != 2) {
        fprintf(stderr, "Anna 1 argumentti: kirjoitettavan tiedoston nimi\n");
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_WRONLY | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
    if(fd < 0) {
        perror("Tiedoston avaaminen epäonnistui: ");
        exit(EXIT_FAILURE);
    }

    
    while(1) {
        
        write(STDOUT_FILENO, (void*)msg, strlen(msg));
        ssize_t s = read(STDIN_FILENO, (void*)buf, MAX_LINE_LENGTH);
        if(s == 0) {
            //EOF
            exit(EXIT_SUCCESS);
        }
        s = write(fd, buf, MAX_LINE_LENGTH);
        if(s == -1) {
            perror("Virhe kirjoittaessa tiedostoon\n");
            exit(EXIT_FAILURE);
        }
    }
    close(fd);

}