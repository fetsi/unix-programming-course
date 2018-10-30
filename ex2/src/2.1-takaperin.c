#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

const size_t step = 100;
char *buf[step];

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Haluan tasan yhden argumentin: tiedostopolku\n");
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Virhe tiedostoa avatessa: ");
        exit(EXIT_FAILURE);
    }

    off_t pos = lseek(fd, 0, SEEK_END);

    printf("Koko: %d tavua\n", (int)pos);

    if(pos < 0) {
        perror("Virhe tiedoston koko hakiessa: ");
        exit(EXIT_FAILURE);
    }
    
    while(pos > 0) {
        pread(fd, buf, step, )
    }



}