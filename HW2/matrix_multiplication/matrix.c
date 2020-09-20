#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#define N (1024)

double** A;
double** B;
double** C;

double calc_time(struct timespec start, struct timespec end) {
    double start_sec = (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec;
    double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec;

    if (end_sec < start_sec) {
        return 0;
    } else {
        return (end_sec - start_sec) / 1000000000.0;
    }
}

void init(void) {
    A = malloc(N * sizeof(*A));
    B = malloc(N * sizeof(*B));
    C = malloc(N * sizeof(*C));
    for(int i = 0; i < N; i++) {
        A[i] = malloc(N * sizeof(*(A[i])));
        B[i] = malloc(N * sizeof(*(B[i])));
        C[i] = malloc(N * sizeof(*(C[i])));
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            A[i][j] = rand() % 100 + 1;
            B[i][j] = rand() % 100 + 2;
            C[i][j] = 0;
        }
    }    
}

double calculate_IJK_time(void) {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    int i, j, k;
    double sum = 0;
    for(i = 0; i < N; i++) {
        for(j = 0; j < N; j++) {
            sum = 0;
            for(k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    return calc_time(start_time, end_time);
}

double calculate_IKJ_time(void) {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    int i, j, k;
    double tmp;
    for(i = 0; i < N; i++) {
        for(k = 0; k < N; k++) {
            tmp = A[i][k];
            for(j = 0; j < N; j++) {
                C[i][j] += tmp * B[k][j];
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    return calc_time(start_time, end_time);
}

double calculate_JKI_time(void) {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    int i, j, k;
    double tmp;
    for(j = 0; j < N; j++) {
        for(k = 0; k < N; k++) {
            tmp = B[k][j];
            for(i = 0; i < N; i++) {
                C[i][j] += tmp * A[i][k];
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    return calc_time(start_time, end_time);
}

void print_msg(const char* title, double elapsed_s) {
    printf("********************%s*********************\n", title);
    printf("Time = %lf s\n\n", elapsed_s);
}

void test_IJK(void) {
    double elapsed_s = calculate_IJK_time();
    print_msg("I-J-K", elapsed_s);
}

void test_IKJ(void) {
    double elapsed_s = calculate_IKJ_time();
    print_msg("I-K-J", elapsed_s);
}

void test_JKI(void) {
    double elapsed_s = calculate_JKI_time();
    print_msg("J-K-I", elapsed_s);
}

void free_all(void) {
    for(int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
}

int main(void) {
    init();
    test_IJK();
    test_IKJ();
    test_JKI();
    free_all();
    return EXIT_SUCCESS;
}