#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define SHM_SIZE 4096
#define TRUE 1
#define FALSE 0

struct person {
    char name[80];
    int age;
};


int main() {
    char *shm_name = "shm_150689";
    int fd;
    sem_t *sem;
    struct person user;
    void *shm_ptr;

    if(sem = sem_open("sem_123", O_CREAT, 0666, 0) == SEM_FAILED) {
        perror("Error opening semaphore: ");
        exit(EXIT_FAILURE);
    }

    if(fd = shm_open(shm_name, O_CREAT|O_RDWR, 0666) == -1) {
        perror("Error opening shared memory: ");
        exit(EXIT_FAILURE);
    }
    if(ftruncate(fd,SHM_SIZE) == -1) {
        perror("Error setting size for shared memory: ");
        exit(EXIT_FAILURE);
    }

    if(shm_ptr = mmap(0, SHM_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0) == MAP_FAILED) {
        perror("Mapping shared memory to processes memory failed: ");
        exit(EXIT_FAILURE);
    }

    if(sem_wait(sem) == -1) {
        perror("Error when waiting for semaphore: ");
        exit(EXIT_FAILURE);
    }

    memcpy((void*)&user, shm_ptr, sizeof(user));

    printf("Name: %s, age: %d\n", user.name, user.age);

    exit(EXIT_SUCCESS);
}