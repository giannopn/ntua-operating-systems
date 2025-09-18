#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int count = 0;
    char cc, c2c = 'a';

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

    // Count char
    c2c = argv[3][0];
    ssize_t rcnt;
    for (;;) {
        rcnt = read(fdr, &cc, 1);
        if (rcnt == -1) {
            perror("Problem reading from file");
            close(fdw);
            close(fdr);
            exit(1);
        }
        if (rcnt == 0) break;  // EOF
        if (cc == c2c) count++;
    }
    close(fdr);

    // Write result in the output file
    char buf[100];
    size_t len, idx = 0;
    ssize_t wcnt;
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