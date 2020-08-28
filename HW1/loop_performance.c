#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>


double calc_time(struct timeval start, struct timeval end) {
    double start_sec = (double)start.tv_sec*1000000.0 + (double)start.tv_usec;
    double end_sec = (double)end.tv_sec*1000000.0 + (double)end.tv_usec;

    if (end_sec < start_sec) {
        return 0;
    } else {
        return end_sec - start_sec;
    }
};

/****************************************************
 *                  Original
 * **************************************************/
// void do_loops(int *a, int *b, int *c, int N) {
//     int i;
//     for (i = N - 1; i >= 1; i--) {
//         a[i] = a[i] + 1;
//     }
//     for (i = 1; i < N; i++) {
//         b[i] = a[i + 1] + 3;
//     }
//     for (i = 1; i < N; i++) {
//         c[i] = b[i - 1] + 2;
//     }
// }


/****************************************************
 *                  Loop Unrolling
 ****************************************************/
// void do_loops(int *a, int *b, int *c, int N) {
//     int i;
//     for (i = N - 1; i >= 1; i -= 9) {
//         a[i] = a[i] + 1;
//         a[i - 1] = a[i - 1] + 1;
//         a[i - 2] = a[i - 2] + 1;
//         a[i - 3] = a[i - 3] + 1;
//         a[i - 4] = a[i - 4] + 1;
//         a[i - 5] = a[i - 5] + 1;
//         a[i - 6] = a[i - 6] + 1;
//         a[i - 7] = a[i - 7] + 1;
//         a[i - 8] = a[i - 8] + 1;
//     }
//     for (i = 1; i < N; i += 9) {
//         b[i] = a[i + 1] + 3;
//         b[i + 1] = a[i + 2] + 3;
//         b[i + 2] = a[i + 3] + 3;
//         b[i + 3] = a[i + 4] + 3;
//         b[i + 4] = a[i + 5] + 3;
//         b[i + 5] = a[i + 6] + 3;
//         b[i + 6] = a[i + 7] + 3;
//         b[i + 7] = a[i + 8] + 3;
//         b[i + 8] = a[i + 9] + 3;
//     }
//     for (i = 1; i < N; i += 9) {
//         c[i] = b[i-1] + 2;
//         c[i + 1] = b[i] + 2;
//         c[i + 2] = b[i + 1] + 2;
//         c[i + 3] = b[i + 2] + 2;
//         c[i + 4] = b[i + 3] + 2;
//         c[i + 5] = b[i + 4] + 2;
//         c[i + 6] = b[i + 5] + 2;
//         c[i + 7] = b[i + 6] + 2;
//         c[i + 8] = b[i + 7] + 2;
//     }
// }


/****************************************************
 *                  Loop Fusion
 * **************************************************/
void do_loops(int *a, int *b, int *c, int N) {
    int i;
    for (i = N - 1; i >= 1; i--) {
        a[i] = a[i] + 1;
    }
    for (i = 1; i < N; i++) {
        b[i] = a[i + 1] + 3;
        c[i] = b[i - 1] + 2;
    }
}

int main(int argc, char *argv[]) {
    int i;
    unsigned long long sum = 0;
    int N;

    if (argc < 2) {
        printf("Usage: ./loop_performance <array_size>\n");
        return -1;
    }

    N = atoi(argv[1]);

    if ((N != 10000000) && (N != 100000000)) {
        printf("Program argument must be 10,000,000 or 100,000,000.\n");
        return -1;
    }

    int *a = malloc((N+1)*sizeof(int));
    int *b = malloc((N+1)*sizeof(int));
    int *c = malloc((N+1)*sizeof(int));

    for (i=0; i<N+1; i++) {
        a[i] = 1;
        b[i] = 0;
        c[i] = 0;
    }

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    do_loops(a, b, c, N);
    gettimeofday(&end_time, NULL);

    for (i=0; i<N; i++) {
        sum += c[i];
    }

    double elapsed_us = calc_time(start_time, end_time);
    double elapsed_ms = elapsed_us / 1000.0;
    printf("sum=%llu\n", sum);
    printf("Time=%f milliseconds\n", elapsed_ms);

    return 0;
}