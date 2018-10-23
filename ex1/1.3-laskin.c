#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

extern int optind;
extern int optopt;
extern char *optarg;
char *endptr;
long result;

int check_validity();

int main(int argc, char *argv[]) {

    

    if(argc < 2) {
        fprintf(stderr, "Et antanut argumenttia etkä valitsimia\n");
        exit(-1);
    }

    while(1) {
       int c = getopt(argc, argv, ":a:s:d:m:");
       if(c == '?') {
            fprintf(stderr, "Tuntematon valitsin: -%c\n", optopt);
            exit(-1);
       }
       if(c == ':') {
            fprintf(stderr, "Valitsimelta puuttuu argumentti\n");
            exit(-1);
       }

        if(c == -1) {
            //Nyt ollaan ensimmäisen argumentin kohdalla
            break;
        }
    }

    errno = 0;
    result = strtol(argv[optind], &endptr, 10);
    if(check_validity() != 0) {
        exit(-1);
    }

    optind = 1;

    while(1) {
       int c = getopt(argc, argv, "a:s:d:m:");
       if(c == -1) {
           //Kaikki valitsimet käsitelty
           break;
       }

       //Parsitaan valitsimen argumentti kokonaisluvuksi ja tehdään virheentarkistus
       errno = 0;
       long oa = strtol(optarg, &endptr, 10);
       if(check_validity() != 0) {
           exit(-1);
       }

       switch(c) {
            case 'a':
                result += oa;
                break;
            case 's':
                result -= oa;
                break;
            case 'd':
                if(oa == 0) {
                    fprintf(stderr, "Ei nollalla saa jakaa!\n");
                    exit(-1);
                }
                result /= oa;
                break;
            case 'm':
                result *= oa;
            default:
                break;
       } 
    }

    printf("%ld\n", result);

    return 0;

}

int check_validity() {
    if(*endptr != '\0') {
        fprintf(stderr, "Kaikkien argumenttien tulee olla kokonaislukuja\n");
        return -1;
    }
    if(errno != 0) {
        perror("Virhe argumentin parsimisessa: ");
        return -1;
    }
    return 0;
}