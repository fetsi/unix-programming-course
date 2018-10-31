#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/param.h>

int fd1, fd2;
off_t f1_size, f2_size;
ssize_t rd_numbytes1, rd_numbytes2;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Anna argumentteina kaksi tiedostopolkua\n");
        exit(EXIT_FAILURE);
    }

    fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        fprintf(stderr, "Tiedoston %s avaaminen epäonnistui: %s\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }
    fd2 = open(argv[2], O_RDONLY);

    if (fd2 == -1) {
        fprintf(stderr, "Tiedoston %s avaaminen epäonnistui: %s\n", argv[2], strerror(errno));
        exit(EXIT_FAILURE);
    }

    f1_size = lseek(fd1, 0, SEEK_END);
    if(f1_size < 0) {
        perror("Virhe tiedostokokoa hakiessa: ");
        exit(EXIT_FAILURE);
    }

    f2_size = lseek(fd2, 0, SEEK_END);
    if(f2_size < 0) {
        perror("Virhe tiedostokokoa hakiessa: ");
        exit(EXIT_FAILURE);
    }

    char *buf1 = malloc(f1_size);
    char *buf2 = malloc(f2_size);

    if(buf1 == NULL || buf2 == NULL) {
        fprintf(stderr, "Muistin varaaminen tiedoston sisällölle epäonnistui\n");
        exit(EXIT_FAILURE);
    }

    lseek(fd1, 0, SEEK_SET);
    lseek(fd2, 0, SEEK_SET);

    rd_numbytes1 = read(fd1, buf1, f1_size);
    if(rd_numbytes1 < 0) {
        perror("Virhe tiedoston lukemisessa: ");
        exit(EXIT_FAILURE);
    }

    rd_numbytes2 = read(fd2, buf2, f2_size);
    if(rd_numbytes2 < 0) {
        perror("Virhe tiedoston lukemisessa: ");
        exit(EXIT_FAILURE);
    }

    ssize_t larger_size = MAX(rd_numbytes1, rd_numbytes2);

for(int i = 0; ; i++) {
    if()
}

}