#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>

#include "landscape.hpp"

#define NUM_ARGV 5

void validate_argc(int argc) {
    if(argc != NUM_ARGV) {
        std::cerr << "Usage: ./rainfall_seq <M> <A> <N> <elevation_file>\n"
            "M = # of simulation time steps during which a rain drop will fall on each landscape point. In other words, 1 rain drop falls on each point during the first M steps of the simulation.\n"
            "A = absorption rate (specified as a floating point number). The amount of raindrops that are absorbed into the ground at a point during a timestep.\n"
            "N = dimension of the landscape (NxN).\n"
            "elevation_file = name of input file that specifies the elevation of each point.\n";
        exit(EXIT_FAILURE);
    }
}

void validate_int_from_str(const char* str, const char* what_str) {
    char* endptr;
    strtol(str, &endptr, 10);
    if(*endptr != '\0') {
        std::cerr << "Wrong " << what_str << ". It should be an integer.\n";
        exit(EXIT_FAILURE);
    }
}

void validate_M(const char* M) {
    validate_int_from_str(M, "simulation time steps");
}

void validate_N(const char* N) {
    validate_int_from_str(N, "dimension of the landscape");
}

void validate_A(const char* A) {
    char* endptr;
    strtod(A, &endptr);
    if(*endptr != '\0') {
        std::cerr << "Wrong absorption rate. It should be a float point number.\n";
        exit(EXIT_FAILURE);
    }
}

void validate_arguments(int argc, char** argv) {
    validate_argc(argc);
    validate_M(argv[1]);
    validate_A(argv[2]);
    validate_N(argv[3]);
}

void output_results(int num_steps, double runtime, const Landscape& landscape) {
    std::cout << "Rainfall simulation completed in " << num_steps << " time steps\n"
    << "Runtime = " << runtime << " seconds\n\n"
    << "The following grid shows the number of raindrops absorbed at each point:\n";
    landscape.print_absorbed_drops();
}

double calc_time(struct timespec start, struct timespec end) { 
    double start_sec = (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec; 
    double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec; 
    if (end_sec < start_sec) {  
        return 0;   
    } else {   
        return end_sec - start_sec;
    } 
}


int main(int argc, char* argv[]) {
    validate_arguments(argc, argv);
    int raining_time = atoi(argv[1]);
    double absorption_rate = atof(argv[2]);
    int dim = atoi(argv[3]);

    Landscape landscape(dim, argv[4]);
    int step = 1;
    int curr_dim = landscape.get_dim();

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    while(step == 1 || !landscape.has_been_dry()) {
        landscape.reset_is_dry();
        // Within one time step
        // First iteration
        for(int i = 0; i < curr_dim; i++) {
            for(int j = 0; j < curr_dim; j++) {
                // Receive raindrops
                if(step <= raining_time) {
                    landscape.receive_rain_drop(i, j);
                }
                // Absorb
                landscape.absorb(i, j, absorption_rate);
                // Calculate trickled drops
                landscape.calculate_trickled_drops(i, j);
            }
        }
        // std::cout << "*************************** trickled drops ******************************\n";
        // landscape.print_trickled_drops();

        // Second iteration
        for(int i = 0; i < curr_dim; i++) {
            for(int j = 0; j < curr_dim; j++) {
                landscape.trickle_to(i, j);
            }
        }
        ++step;
        // std::cout << "****************************** raindrops ***************************\n";
        // landscape.print_raindrops();
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double runtime = calc_time(start_time, end_time) / 1000000000.0;
    output_results(step - 1, runtime, landscape);
    
    return EXIT_SUCCESS;
}