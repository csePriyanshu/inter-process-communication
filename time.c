#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char* argv[]) 
{
	struct timespec start, end;
	double time_elapsed;

	if (argc < 2)
	{
        	printf("Usage: %s [args]\n", argv[0]);
        	exit(1);
    	}
	clock_gettime(CLOCK_MONOTONIC, &start);  // getting more precise time (in nsec)
	pid_t pid = fork();
	if(pid ==0)
	{
	    	//printf("Executing: %s\n", argv[1]);

    		// Execute the program specified in argv[1] with any additional arguments
	    	execv(argv[1], &argv[1]);

    		// If execv fails, print an error and exit
	    	perror("execv failed");
		exit(1);
	}
	else
	{
		wait(NULL);  // wait for the child process to return
		clock_gettime(CLOCK_MONOTONIC, &end);
		time_elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;
		printf("Elapsed time (in s): %f\n", time_elapsed);
		//printf("Child process returned\n");
	}

    	return 1;
}

