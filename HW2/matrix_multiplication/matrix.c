#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#define N (1024)
#define SUB_BLOCK_SIZE (128)

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

void clear_C(void) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
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

int min(int a, int b) {
    return a < b ? a : b;
}

double calculate_IJK_loop_tiling_time(void) {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    int i, j, k, ii, jj, kk;
    double tile_sum;
    for(i = 0; i < N; i += SUB_BLOCK_SIZE) {
        for(j = 0; j < N; j += SUB_BLOCK_SIZE) {
            for(k = 0; k < N; k += SUB_BLOCK_SIZE) {
                for(ii = i; ii < i + SUB_BLOCK_SIZE; ii++) {
                    for(jj = j; jj < j + SUB_BLOCK_SIZE; jj++) {
                        tile_sum = 0;
                        for(kk = k; kk < k + SUB_BLOCK_SIZE; kk++) {
                            tile_sum += A[ii][kk] * B[kk][jj];
                        }
                        C[ii][jj] += tile_sum;  // need to init C beforehand
                    }
                }
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

void test_IJK_loop_tiling(void) {
    clear_C();
    double elapsed_s = calculate_IJK_loop_tiling_time();
    print_msg("I-J-K with loop filing", elapsed_s);
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

void test_all(void) {
    test_IJK();
    test_IKJ();
    test_JKI();
}

int main(int argc, char *argv[]) {
    init();

    if(argc == 1) {
        test_all();
    } else if(argc != 2) {
        printf("Usage:\n(1) ./matrix to run all the three kinds of orderings.\n"
        "(2) ./matrix <MODE> to run one specific ordering.\n\tMode1: I-J-K; Mode2: I-K-J; Mode3: J-K-I; Mode4: I-J-K with loop tiling\n");
        return EXIT_FAILURE;
    } else {
        int mode = atoi(argv[1]);
        switch (mode) {
            case 1:
                test_IJK();
                break;
            case 2:
                test_IKJ();
                break;
            case 3:
                test_JKI();
                break;
            case 4:
                test_IJK_loop_tiling();
                break;
            default:
                printf("Only 4 modes are available:\nMode1: I-J-K; Mode2: I-K-J; Mode3: J-K-I; Mode4: I-J-K with loop tiling\n");
                return EXIT_FAILURE;
        }
    }

    free_all();
    return EXIT_SUCCESS;
}