#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#define P 3  // Number of child processes

int active_children = 0;

// Signal handler for SIGINT
void sighandler(int signum) {
    printf("\nActive search processes: %d\n", active_children);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        perror("Wrong number of arguments");
        exit(1);
    }

    char c2c = argv[3][0];

    // Open file for reading
    int fdr = open(argv[1], O_RDONLY);
    if (fdr == -1) {
        perror("Problem opening file to read");
        exit(1);
    }

    // Get file size
    off_t filesize = lseek(fdr, 0, SEEK_END);
    if (filesize == -1) {
        perror("Problem getting file size");
        close(fdr);
        exit(1);
    }
    close(fdr);

    int pipes[P][2];
    for (int i = 0; i < P; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Problem creating pipe");
            close(fdr);
            exit(1);
        }
    }

    off_t sliced_size = filesize / P;

    for (int i = 0; i < P; i++) {
        active_children++;

        pid_t pid = fork();
        if (pid < 0) {
            perror("Problem creating child process");
            exit(1);
        }
        if (pid == 0) {
            signal(SIGINT, SIG_IGN);  // Ignore SIGINT in child processes
            close(pipes[i][0]);

            int fdr = open(argv[1], O_RDONLY);
            if (fdr == -1) {
                perror("Problem opening file to read");
                exit(1);
            }

            off_t start_index = i * sliced_size;
            off_t end_index = (i == P - 1) ? filesize : (i + 1) * sliced_size;

            // Set file offset to start_index
            if (lseek(fdr, start_index, SEEK_SET) == -1) {
                perror("Problem setting file offset");
                close(fdr);
                exit(1);
            }

            int child_count = 0;
            char cc;

            // Read from start_index to end_index
            for (off_t index = start_index; index < end_index; index++) {
                if (read(fdr, &cc, 1) == -1) {
                    perror("Problem reading from file");
                    close(fdr);
                    exit(1);
                }
                if (cc == c2c) child_count++;
            }

            // Write child_count to pipe
            if (write(pipes[i][1], &child_count, sizeof(child_count)) != sizeof(child_count)) {
                perror("Problem writing to pipe");
                close(fdr);
                exit(1);
            }

            close(fdr);
            close(pipes[i][1]);
            exit(0);
        }
        close(pipes[i][1]);
    }

    signal(SIGINT, sighandler);  // Set signal handler for SIGINT
    int total_count = 0;

    // Wait for all children to finish
    for (int i = 0; i < P; i++) {
        wait(NULL);
        active_children--;
    }

    // Read child_count from pipes and calculate total_count
    for (int i = 0; i < P; i++) {
        int child_count;
        if (read(pipes[i][0], &child_count, sizeof(child_count)) != sizeof(child_count)) {
            perror("Problem reading from pipe");
            exit(1);
        }
        close(pipes[i][0]);
        total_count += child_count;
    }

    printf("Total count: %d\n", total_count);

    // Write result to output file
    int oflags = O_WRONLY | O_CREAT | O_TRUNC;
    int mode = S_IRUSR | S_IWUSR;
    int fdw = open(argv[2], oflags, mode);
    if (fdw == -1) {
        perror("Problem opening file to write");
        close(fdr);
        exit(1);
    }

    char buf[100];
    size_t len, idx = 0;
    ssize_t wcnt;
    len = sprintf(buf, "The character '%c' appears %d times in file %s.\n",
        c2c, total_count, argv[1]);
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