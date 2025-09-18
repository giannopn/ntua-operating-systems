#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
    pid_t p, mypid, parentpid;
    int status;

    if (argc != 4) {
        perror("Wrong number of arguments");
        exit(1);
    }

    p = fork();
    if (p < 0) {
        perror("Problem creating child process");
        exit(1);
    }
    if (p == 0) {
        mypid = getpid();
        parentpid = getppid();
        printf("Hello from child process. My pid is %d and my parent's pid is %d\n",
            mypid, parentpid);

        char *args[] = {"./main-source-code", argv[1], argv[2], argv[3], NULL};
        execv(args[0], args);

        // If execv returns, there was an error
        perror("Problem executing child process");
        exit(1);
    }

    printf("Child process created with pid %d\n", p);
    wait(&status);
    return 0;
}