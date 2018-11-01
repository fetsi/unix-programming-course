#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/param.h>
#include <string.h>

int fd1, fd2;
size_t f1_size, f2_size;
size_t rd_numbytes1, rd_numbytes2;
char *buf1, *buf2;

//Funktioprototyypit
int open_file(char *path);
size_t read_file(int fd, char *buf, size_t count);
size_t get_size(int fd);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Anna argumentteina kaksi tiedostopolkua\n");
        exit(EXIT_FAILURE);
    }

    //Avaa tiedostot
    fd1 = open_file(argv[1]);
    fd2 = open_file(argv[2]);
    //Hae tiedostojen koot tavuissa
    f1_size = get_size(fd1);
    f2_size = get_size(fd2);
    //Varaa heapista taulukot tiedostojen sisällöille
    buf1 = malloc(f1_size + 1);
    buf2 = malloc(f2_size + 1);

    if(buf1 == NULL || buf2 == NULL) {
        fprintf(stderr, "Muistin varaaminen tiedoston sisällölle epäonnistui\n");
        exit(EXIT_FAILURE);
    }
    
    //Täytä nolla-merkillä
    memset((void*)buf1, '\0', f1_size + 1);
    memset((void*)buf2, '\0', f2_size + 1);

    //Lue tiedostot muistiin
    rd_numbytes1 = read_file(fd1, buf1, f1_size);
    rd_numbytes2 = read_file(fd2, buf2, f2_size);

   //Käy merkkitaulukot läpi merkki merkiltä 
    for(int i = 0; ; i++) {
        if( (buf1[i] != buf2[i]) || buf1[i] == '\0' || buf2[i] == '\0' )  {
            printf("Ensimmäinen eroava merkki on merkki nro %d. Tiedosto 1: %c, Tiedosto 2: %c\n", i+1, buf1[i], buf2[i] );
            break;
        }
    }
    free(buf1);
    free(buf2);

    return 0;
}

/*Avaa tiedoston polussa path ja palauttaa tiedostokuvaajan*/
int open_file(char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Tiedoston %s avaaminen epäonnistui: %s\n", path, strerror(errno));
        free(buf1);
        free(buf2);
        exit(EXIT_FAILURE);
    }
    return fd;
}

/*Lukee tiedostosta fd count-muuttujan verran tavuja annettuun puskuriin*/
size_t read_file(int fd, char *buf, size_t count) {
    int rd_numbytes = read(fd, buf, count);
    if(rd_numbytes < 0) {
        perror("Virhe tiedoston lukemisessa: ");
        free(buf1);
        free(buf2);
        exit(EXIT_FAILURE);
    }
    return rd_numbytes;
}


/*Palauttaa tiedoston koon tavuina. Jättää lopuksi osoittimen tiedoston alkuun*/
size_t get_size(int fd) {
    off_t size = lseek(fd, 0, SEEK_END);
    if(size < 0) {
        perror("Virhe tiedostokokoa hakiessa: ");
        free(buf1);
        free(buf2);
        exit(EXIT_FAILURE);
    }
    lseek(fd, 0, SEEK_SET);
    return (size_t)size;
}

