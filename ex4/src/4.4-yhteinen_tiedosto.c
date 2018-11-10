#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

char filename[] = "/tmp/my_temp_fileXXXXXX";
int child_ex_status;

int main() {

    int fd = mkstemp(filename);
    if (fd == -1) {
        perror("Error creating temp file: ");
        exit(EXIT_FAILURE);
    }

    pid_t new_pid = fork();
    if(new_pid == -1) {
        perror("fork() failed: ");
        exit(EXIT_FAILURE);
    }
    if(new_pid == 0) {
        //In the child
        const char *buf = "This is a line\nThis is another line\nAnd a third one";
        int wr = write(fd, (const void*)buf, strlen(buf) );
        if(wr == -1) {
            perror("Error writing to file from child: ");
            exit(EXIT_FAILURE);
        }
        printf("%d bytes written to file by child process\n", wr);
        /*We can already unlink the file here, because this just removes the pathname from the filesystem.
        The parent process still has a file descriptor pointing to the file, so it can still operate on the file
        as long as it doesn't close the fd
        */
        unlink(filename);                                     
        exit(EXIT_SUCCESS);
    }

    if(wait(&child_ex_status) == -1) {
        perror("Error waiting on child: ");
        exit(EXIT_FAILURE);
    }

    if(WIFEXITED(child_ex_status)) {
        printf("Child process exited with status %d\n", WEXITSTATUS(child_ex_status));
    }

    //Get the size of the file
    off_t size = lseek(fd, 0, SEEK_END);
    char *read_buf = calloc(size + 1, 1);

    
    //Move the file position back to beginning
    lseek(fd, 0, SEEK_SET);

    //Read contents
    int rd = read(fd, read_buf, size);
    if(rd == -1) {
        perror("Failed to read file from parent process\n");
        exit(EXIT_FAILURE);
    }
    close(fd);
    
    printf("%d bytes read from file by parent process\n", rd);
    printf("%s\n", read_buf);

    return 0;

}