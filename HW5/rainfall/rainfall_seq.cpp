#include <iostream>
#include <cstdlib>
#include <string>

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

int main(int argc, char* argv[]) {
    validate_arguments(argc, argv);

}