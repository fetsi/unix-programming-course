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
        printf("Anna kokonaisuku: ");
        if(scanf("%d", &new) == EOF) {
            break;
        }
        snprintf(buf, MAX_LINE_LENGTH, "%d\n", new);
        size_t len = strlen(buf);
        write(fd, (void*)buf, len);
    }
    close(fd);

}