#include <stdio.h>


int main(int argc, char *argv[]) {

    printf("Ohjelman nimi on %s\n", argv[0]);
    printf("Annoit ohjelman nimen perässä %d komentoriviargumenttia\n", argc -1);

    for (int i = argc -1; i >= 0; i--) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    return 0;

}