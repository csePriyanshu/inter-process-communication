#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define SHM_NAME "/my_shm" // Name of the shared memory object
#define SHM_SIZE sizeof(struct timespec)

int main(int argc, char* argv[])
{
    int shm_fd;
    void* shm_ptr;
    pid_t pid;
    struct timespec start, end, *child_start;
    double time_elapsed;

    if (argc < 2)
    {
        printf("Usage: %s <program_to_exec> [args]\n", argv[0]);
        exit(1);
    }

    // Create or open the shared memory object
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(1);
    }

    // Configure the size of the shared memory object
    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    // Map the shared memory object into the process's address space
    shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    // Get the start time before forking
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Fork a new process
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        // Child process
        child_start = (struct timespec*) shm_ptr;

        // Note the start time in the child process
        clock_gettime(CLOCK_MONOTONIC, child_start);

        // Execute the program specified in argv[1] with any additional arguments
        execv(argv[1], &argv[1]);

        // If execv fails, print an error and exit
        perror("execv failed");
        exit(1);
    }
    else
    {
        // Parent process
        // Wait for the child process to return
        wait(NULL);

        // Get the end time after the child process finishes
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Read the start time from shared memory
        child_start = (struct timespec*) shm_ptr;

        // Calculate the elapsed time
        time_elapsed = (end.tv_sec - child_start->tv_sec) + (end.tv_nsec - child_start->tv_nsec) / 1e9;
        printf("Elapsed time (in s): %f\n", time_elapsed);

        // Unmap the shared memory
        if (munmap(shm_ptr, SHM_SIZE) == -1)
        {
            perror("munmap");
            exit(1);
        }

        // Close and unlink the shared memory object
        close(shm_fd);
        shm_unlink(SHM_NAME);
    }

    return 0;
}

