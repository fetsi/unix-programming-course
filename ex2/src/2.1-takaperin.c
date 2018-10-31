#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

char *buf;

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


    size_t size = (size_t) pos;

    buf = malloc(size);

    if(buf == NULL) {
        perror("Muistin varaaminen tiedoston sisällölle epäonnistui: ");
        exit(EXIT_FAILURE);
    }

    pos = lseek(fd, 0, SEEK_SET);

    ssize_t rd_numbytes = read(fd, buf, size);
    
    if(rd_numbytes == -1) {
        perror("Tiedoston luku epäonistui: ");
        exit(EXIT_FAILURE);
    }

    for(ssize_t i = rd_numbytes -1; i >= 0; i-- ) {
        printf("%c", buf[i]);
    }
    printf("\n");

}