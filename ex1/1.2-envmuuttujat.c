#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

const char *value = "EI_OLE_ASETETTU";
extern char **environ;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Haluan tasan yhden komentoriviargumentin!\n");
        return -1;
    }

    char *contents = getenv(argv[1]);

    if (contents != NULL) {
        printf("%s=%s\n", argv[1], contents);
    }

    else {
        if(setenv(argv[1], value
    , 1) != 0) {
            fprintf(stderr, "Virhe ympäristömuuttujan asettamisessa: %s\n", strerror(errno));
            return -1;
        }

        char **ptr = environ;

        while(*ptr != NULL) {
            printf("%s\n", *ptr);
            ptr++;
        } 
        return 0;

    }





}