#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv) {
    // Create pipe
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr,"Pipe failed\n");
        return 1;
    }

    // Storing time
    struct timeval before, after, rewrite;

    // For child process
    pid_t pid;
    pid = fork();

    // Error creating child
    if (pid < 0) {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    // Parent process
    if (pid > 0) {
        // Wait for completion of child
        wait(NULL);
        // Close write end and get end time
        close(fd[WRITE_END]);
        gettimeofday(&after, NULL);
        // Get start time from pipe and subtract from start time to show total time
        read(fd[READ_END], &rewrite, sizeof(rewrite));
        float time = (after.tv_usec + after.tv_sec*1000000) - (rewrite.tv_usec + rewrite.tv_sec*1000000);
        printf("Elapsed time: %.5f\n", time/1000000);
        // Close read end
        close(fd[READ_END]);
    }
    // Child process
    else {
        // Close read end and write start time to pipe
        close(fd[READ_END]);
        gettimeofday(&before, NULL);
        write(fd[WRITE_END], &before, sizeof(before));
        // Close write end and execute argument
        close(fd[WRITE_END]);        
        execvp(argv[1], &argv[1]);
    }
    return 0;
}