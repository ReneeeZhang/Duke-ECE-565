#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t *array;
int num_elements;  // L1 cache size is 32KB, each array element is 8B, thus, all cache loads can happen within L1 if num_elements <= 4K
int num_traversals;

double calc_time(struct timespec start, struct timespec end) {
    double start_sec = (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec;
    double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec;

    if (end_sec < start_sec) {
        return 0;
    } else {
        return end_sec - start_sec;
    }
}

void init_array(void) {
    for (int i = 0; i < num_elements; i++) {
        array[i] = rand() % 100;
    }
}

double calculate_write_only_time() {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    for(int i = 0; i < num_traversals; i++) {
        for(int j = 0; j < num_elements; j++) {
            array[j] = 8;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    return calc_time(start_time, end_time);
}

double calculate_read_write_1_1_time() {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    for(int i = 0; i < num_traversals; i++) {
        for(int j = 0; j < num_elements; j++) {
            array[j] = array[j] + 8;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    return calc_time(start_time, end_time);
}

double calculate_read_write_2_1_time() {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    for(int i = 0; i < num_traversals; i++) {
        for(int j = 0; j < num_elements - 1; j++) {
            array[j] = array[j] + array[j + 1];
        }
        array[num_elements - 1] = array[num_elements - 1] + array[0];
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    return calc_time(start_time, end_time);
}

void print_msg(const char* title, double elapsed_ns) {
    printf("********************%s*********************\n", title);
    printf("Time = %lf ns\n", elapsed_ns);
    printf("Bandwidth is = %lf GB/s\n", ((double)num_elements * num_traversals * 8) / (elapsed_ns));
    printf("\n");
}

void test_write_only() {
    double elapsed_ns = calculate_write_only_time();
    print_msg("Write traffic only", elapsed_ns);
}

void test_read_write_1_1() {
    double elapsed_ns = calculate_read_write_1_1_time();
    print_msg("1:1 read-to-write traffic", elapsed_ns);
}

void test_read_write_2_1() {
    double elapsed_ns = calculate_read_write_2_1_time();
    print_msg("2:1 read-to-write traffic", elapsed_ns);
}

int main(int argc, char *argv[]) {
    num_elements = atoi(argv[1]);
    num_traversals = atoi(argv[2]);
    
    array = malloc(num_elements * sizeof(*array));
    init_array();

    test_write_only();
    test_read_write_1_1();
    test_read_write_2_1();

    free(array);
}