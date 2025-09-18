#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t p, mypid, parentpid;
    int status;

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
        exit(0);
    }

    printf("Child process created with pid %d\n", p);
    wait(&status);
    return 0;
}