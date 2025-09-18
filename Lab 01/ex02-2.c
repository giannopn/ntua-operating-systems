#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t p, mypid, parentpid;
    int status, x = 0;

    p = fork();
    if (p < 0) {
        perror("Problem creating child process");
        exit(1);
    }
    if (p == 0) {
        mypid = getpid();
        parentpid = getppid();
        x = 1;
        printf("Hello from child process. My pid is %d and my parent's pid is %d\n",
            mypid, parentpid);
        printf("Child: x = %d\n", x);
        exit(0);
    }

    x = 2;
    printf("Child process created with pid %d\n", p);
    printf("Parent: x = %d\n", x);
    wait(&status);
    return 0;
}