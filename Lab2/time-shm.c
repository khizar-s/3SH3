#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char **argv) {
    // For shared memory
    const int SIZE = 4096;
    const char *name = "OS";
    int fd;
    char *ptr;
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);
    ptr = (char *)
    mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // For child process
    pid_t pid;
    pid = fork();

    // Storing time
    struct timeval before, after;

    // Error creating child
    if (pid < 0) {
        fprintf(stderr, "Unable to create child processes \n");
        return 1;
    }
    // Child process
    else if (pid == 0) {
        // Get start time and add to shared memory
        gettimeofday(&before, NULL);
        char convert[20];
        sprintf(convert, "%ld", before.tv_usec + before.tv_sec*1000000);
        sprintf(ptr, "%s", convert);
        ptr += strlen(convert);
        // Execute argument
        execvp(argv[1], &argv[1]);
    }
    // Parent process
    else {
        // Wait for completion of child
        wait(NULL);
        // Take end time and subtract from start time to show total time
        gettimeofday(&after, NULL);
        float time = after.tv_usec + after.tv_sec*1000000 - atof((char *)ptr);
        printf("Elapsed time: %.5f\n", time/1000000);
        // Remove shared memory
        shm_unlink(name);
        return 0;
    }
}