#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    pid_t p, mypid, parentpid;
    int status;

    if (argc != 4) {
        perror("Wrong number of arguments");
        exit(1);
    }

    // Open file for reading
    int fdr = open(argv[1], O_RDONLY);
    if (fdr == -1) {
        perror("Problem opening file to read");
        exit(1);
    }

    // Open file for writing the result
    int oflags = O_WRONLY | O_CREAT | O_TRUNC;
    int mode = S_IRUSR | S_IWUSR;
    int fdw = open(argv[2], oflags, mode);
    if (fdw == -1) {
        perror("Problem opening file to write");
        close(fdr);
        exit(1);
    }

    // Create pipe
    int pfd[2];
    if (pipe(pfd) == -1) {
        perror("Problem creating pipe");
        close(fdr);
        close(fdw);
        exit(1);
    }

    p = fork();
    if (p < 0) {
        perror("Problem creating child process");
        close(fdr);
        close(fdw);
        exit(1);
    }
    if (p == 0) {
        close(pfd[0]);

        mypid = getpid();
        parentpid = getppid();
        printf("Hello from child process. My pid is %d and my parent's pid is %d\n",
            mypid, parentpid);

        int count = 0;
        char cc, c2c = 'a';
        c2c = argv[3][0];

        // Count char
        ssize_t rcnt;
        for (;;) {
            rcnt = read(fdr, &cc, 1);
            if (rcnt == -1) {
                perror("Problem reading from file");
                close(fdr);
                close(fdw);
                exit(1);
            }
            if (rcnt == 0) break;  // EOF
            if (cc == c2c) count++;
        }

        // Write count to pipe
        if (write(pfd[1], &count, sizeof(count)) != sizeof(count)) {
            perror("Problem writing to pipe");
            close(fdr);
            close(fdw);
            exit(1);
        }

        close(pfd[1]);
        exit(0);
    }

    close(fdr);
    close(pfd[1]);
    printf("Child process created with pid %d\n", p);
    wait(&status);

    // Read count from pipe
    int count;
    if (read(pfd[0], &count, sizeof(count)) != sizeof(count)) {
        perror("Problem reading from pipe");
        close(fdw);
        exit(1);
    }
    close(pfd[0]);

    // Write result in the output file
    char buf[100];
    size_t len, idx = 0;
    ssize_t wcnt;
    char c2c = argv[3][0];
    len = sprintf(buf, "The character '%c' appears %d times in file %s.\n",
        c2c, count, argv[1]);
    do {
        wcnt = write(fdw, buf + idx, len - idx);
        if (wcnt == -1) {
            perror("Problem writing to file");
            close(fdw);
            exit(1);
        }
        idx += wcnt;
    } while (idx < len);
    close(fdw);

    return 0;
}