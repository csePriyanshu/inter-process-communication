#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

#define BUFFER_SIZE 128

int main(int argc, char* argv[])
{
    int fd[2];
    pid_t pid;
    struct timespec start, end, child_start;
    double time_elapsed;
    ssize_t bytes_read;

    if (argc < 2)
    {
        printf("Usage: %s <program_to_exec> [args]\n", argv[0]);
        exit(1);
    }

    // Create a pipe
    if (pipe(fd) == -1)
    {
        perror("pipe");
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
        close(fd[0]); // Close the read end of the pipe

        // Note the start time in the child process
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Send the start time to the parent process
        if (write(fd[1], &start, sizeof(start)) != sizeof(start))
        {
            perror("write failed");
            exit(1);
        }

        // Close the write end of the pipe
        close(fd[1]);

        // Execute the program specified in argv[1] with any additional arguments
        execv(argv[1], &argv[1]);

        // If execv fails, print an error and exit
        perror("execv failed");
        exit(1);
    }
    else
    {
        // Parent process
        close(fd[1]); // Close the write end of the pipe

        // Read the start time from the pipe
        bytes_read = read(fd[0], &child_start, sizeof(child_start));
        if (bytes_read != sizeof(child_start))
        {
            perror("read failed");
            exit(1);
        }

        // Wait for the child process to return
        wait(NULL);

        // Get the end time after the child process finishes
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate the elapsed time
        time_elapsed = (end.tv_sec - child_start.tv_sec) + (end.tv_nsec - child_start.tv_nsec) / 1e9;
        printf("Elapsed time (in s): %f\n", time_elapsed);

        // Close the read end of the pipe
       	close(fd[0]);
    }

    return 0;
}

