#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>

#define P 3  // Number of child processes

static sem_t *sem;

int active_children = 0;
int *total_count;

// Signal handler for SIGINT
void sighandler(int signum) {
    printf("\nActive search processes: %d\n", active_children);
    printf("Total count now: %d\n", *total_count);
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

    off_t sliced_size = filesize / P;

    total_count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (total_count == MAP_FAILED) {
        perror("Problem mapping memory");
        exit(1);
    }
    *total_count = 0;
    sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sem == MAP_FAILED) {
        perror("Problem mapping semaphore memory");
        exit(1);
    }
    sem_init(sem, 1, 1);

    for (int i = 0; i < P; i++) {
        active_children++;

        pid_t pid = fork();
        if (pid < 0) {
            perror("Problem creating child process");
            exit(1);
        }
        if (pid == 0) {
            signal(SIGINT, SIG_IGN);  // Ignore SIGINT in child processes

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

            char cc;

            // Read from start_index to end_index
            for (off_t index = start_index; index < end_index; index++) {
                if (read(fdr, &cc, 1) == -1) {
                    perror("Problem reading from file");
                    close(fdr);
                    exit(1);
                }
                if (cc == c2c) {
                    sem_wait(sem);
                    *total_count += 1;
                    sem_post(sem);
                }
            }

            //sem_wait(&sem);
            //*total_count += child_count;
            //sem_post(&sem);

            exit(0);
        }
    }

    signal(SIGINT, sighandler);  // Set signal handler for SIGINT

    // Wait for all children to finish
    for (int i = 0; i < P; i++) {
        wait(NULL);
        active_children--;
    }

    printf("Total count: %d\n", *total_count);

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
        c2c, *total_count, argv[1]);
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

    if (munmap(total_count, sizeof(int)) == -1) {
        perror("munmap");
        exit(1);
    }
    if (sem_destroy(sem) == -1) {
        perror("sem_destroy");
        exit(1);
    }
    if (munmap(sem, sizeof(sem_t)) == -1) {
        perror("munmap sem");
        exit(1);
    }
    return 0;
}