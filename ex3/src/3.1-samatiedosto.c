#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int open_file(char *path);
int sameFile(int fd1, int fd2);

int main() {

    //Testi 1: Avataan tiedosto, kopioidaan sen tiedostokuvaaja ja verrataan näitä kahta kuvaajaa
    int fd1 = open_file("./testi");
    int fd2 = dup(fd1);

    if(sameFile(fd1, fd2)) {
        printf("Testi 1: Duplikoitu tiedostokuvaaja viittaa samaan tiedostoon kuin alkuperäinen\n");
    } else {
        printf("Testi 1: Duplikoitu tiedostokuvaaja viittaa eri tiedostoon kuin alkuperäinen\n");
    }

    //Testi 2: Tehdään kova linkki tiedostoon "testi" ja verrataan sitä alkuperäiseen
    link("./testi", "./kovalinkki_testiin");
    //int fd3 = open_file("./kovalinkki_testiin");
    int fd3 = open("./kovalinkki_testiin", 0);
    if(fd3 == -1) {
        perror(":");
    }
    if(sameFile(fd1, fd3)) {
        printf("Testi 2: Kova linkki viittaa samaan tiedostoon kuin alkuperäinen\n");
    }
    else {
        printf("Testi 2: Kova linkki viittaa eri tiedostoon kuin alkuperäinen\n");
    }

    //Testi 3: Luodaan symbolinen linkki tiedostoon ja verrataan sitä alkuperäiseen
    symlink("./testi", "./symlinkki");
    int fd4 = open_file("./symlinkki");
    if(sameFile(fd1, fd4)) {
        printf("Testi 3: Symbolinen linkki viittaa samaan tiedostoon kuin alkuperäinen\n");
    } else {
        printf("Testi 3: Symbolinen linkki viittaa eri tiedostoon kuin alkuperäinen\n");
    }

    return 0;
}


int sameFile(int fd1, int fd2) {
    struct stat file1_info, file2_info;

    if(fstat(fd1, &file1_info) < 0  || fstat(fd2, &file2_info) < 0) {
        perror("fstat() epäonnistui: ");
        exit(EXIT_FAILURE);
    }

    //Jos inodet ovat samat ja device-numerokin täsmää, (eli ovat samassa tiedostojärjestelmässä), tiedostokuvaajat viittaavat samaan fyysiseen tiedostoon
    if(file1_info.st_ino == file2_info.st_ino && file1_info.st_dev == file2_info.st_dev) {
        return TRUE;
    }
    return FALSE;
}

/*Avaa tiedoston polussa path ja palauttaa tiedostokuvaajan. Luodaan tiedosto, jos sitä ei ole jo olemassa*/
int open_file(char *path) {
    int fd = open(path, O_RDONLY| O_CREAT | S_IRWXU | S_IRGRP | S_IROTH);
    if (fd == -1) {
        fprintf(stderr, "Tiedoston %s avaaminen epäonnistui: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fd;
}

