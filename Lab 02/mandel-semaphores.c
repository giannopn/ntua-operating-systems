/*
 * mandel.c
 *
 * A program to draw the Mandelbrot Set on a 256-color xterm.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>

#include "mandel-lib.h"

#define MANDEL_MAX_ITERATION 100000

#define perror_pthread(ret, msg) \
	do { errno = ret; perror(msg); } while (0)

/***************************
 * Compile-time parameters *
 ***************************/

/*
 * Output at the terminal is is x_chars wide by y_chars long
*/
int y_chars = 50;
int x_chars = 90;

/*
 * The part of the complex plane to be drawn:
 * upper left corner is (xmin, ymax), lower right corner is (xmax, ymin)
*/
double xmin = -1.8, xmax = 1.0;
double ymin = -1.0, ymax = 1.0;

/*
 * Every character in the final output is
 * xstep x ystep units wide on the complex plane.
 */
double xstep;
double ystep;

static sem_t *sems;
static int nthreads;

/*
 * This function computes a line of output
 * as an array of x_char color values.
 */
void compute_mandel_line(int line, int color_val[])
{
	/*
	 * x and y traverse the complex plane.
	 */
	double x, y;

	int n;
	int val;

	/* Find out the y value corresponding to this line */
	y = ymax - ystep * line;

	/* and iterate for all points on this line */
	for (x = xmin, n = 0; n < x_chars; x+= xstep, n++) {

		/* Compute the point's color value */
		val = mandel_iterations_at_point(x, y, MANDEL_MAX_ITERATION);
		if (val > 255)
			val = 255;

		/* And store it in the color_val[] array */
		val = xterm_color(val);
		color_val[n] = val;
	}
}

/*
 * This function outputs an array of x_char color values
 * to a 256-color xterm.
 */
void output_mandel_line(int fd, int color_val[])
{
	int i;

	char point ='@';
	char newline='\n';

	for (i = 0; i < x_chars; i++) {
		/* Set the current color, then output the point */
		set_xterm_color(fd, color_val[i]);
		if (write(fd, &point, 1) != 1) {
			perror("compute_and_output_mandel_line: write point");
			exit(1);
		}
	}

	/* Now that the line is done, output a newline character */
	if (write(fd, &newline, 1) != 1) {
		perror("compute_and_output_mandel_line: write newline");
		exit(1);
	}
}

void compute_and_output_mandel_line(int fd, int line)
{
	/*
	 * A temporary array, used to hold color values for the line being drawn
	 */
	int color_val[x_chars];

	compute_mandel_line(line, color_val);
	output_mandel_line(fd, color_val);
}

static void *compute_and_output(void *arg) {
	int tid = *(int*)arg;
	free(arg);

	int color_val[x_chars];

	for (int line = tid; line < y_chars; line += nthreads) {
		compute_mandel_line(line, color_val);

		if (sem_wait(&sems[tid]) == -1) {
			perror("sem_wait");
			exit(1);
		}

		output_mandel_line(1, color_val);

		int next_tid = (tid + 1) % nthreads;
		if (line + 1 < y_chars) {
			if (sem_post(&sems[next_tid]) == -1) {
				perror("sem_post");
				exit(1);
			}
		}
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <NTHREADS>\n", argv[0]);
		exit(1);
	}

	nthreads = atoi(argv[1]);

	if (nthreads < 1) {
		fprintf(stderr, "NTHREADS must be >= 1\n");
		exit(1);
	}

	sems = malloc(nthreads * sizeof(sem_t));
	if (!sems) {
		perror("malloc sems");
		exit(1);
	}

	for (int i = 0; i < nthreads; i++) {
        if (sem_init(&sems[i], 1, i == 0 ? 1 : 0) == -1) {
            perror("sem_init");
            exit(1);
        }
    }

	xstep = (xmax - xmin) / x_chars;
	ystep = (ymax - ymin) / y_chars;


	pthread_t *tids = malloc(sizeof(pthread_t) * nthreads);
	if (!tids) {
		perror("malloc tids");
		exit(1);
	}

	for (int i = 0; i < nthreads; i++) {
		int *p = malloc(sizeof *p);
		if (!p) {
			perror("malloc p");
			exit(1);
		}
		*p = i;
		int ret = pthread_create(&tids[i], NULL, compute_and_output, p);
		if (ret) {
			perror_pthread(ret, "pthread_create");
			exit(1);
		}
	}

	for (int i = 0; i < nthreads; i++) {
		int ret = pthread_join(tids[i], NULL);
		if (ret) {
			perror_pthread(ret, "pthread_join");
			exit(1);
		}
	}

	//cleanup_semaphores(nthreads);
	for (int i = 0; i < nthreads; i++) {
        if (sem_destroy(&sems[i]) == -1) {
            perror("sem_destroy");
            exit(1);
        }
    }
	free(tids);
	reset_xterm_color(1);
	return 0;
}
