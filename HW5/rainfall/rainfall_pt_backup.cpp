#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/ref.hpp>
#include <boost/fiber/barrier.hpp>

#include "landscape.hpp"

#define NUM_ARGV 6

static int num_threads;
static int raining_time;
static double absorption_rate;
static int dim;

void validate_argc(int argc) {
    if(argc != NUM_ARGV) {
        std::cerr << "Usage: ./rainfall_seq <P> <M> <A> <N> <elevation_file>\n"
            "P = # of parallel threads to use.\n"
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

void validate_P(const char* P) {
    validate_int_from_str(P, "simulation time steps");
}

void validate_M(const char* M) {
    validate_int_from_str(M, "simulation time steps");
}

void validate_N(const char* N) {
    validate_int_from_str(N, "dimension of the landscape");
    int dim = atoi(N);
    if(dim < 32) {
        std::cerr << "To run parallel edition, the dimension of the landscape should be no smaller than 32.\n";
    }
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
    validate_P(argv[1]);
    validate_M(argv[2]);
    validate_A(argv[3]);
    validate_N(argv[4]);
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

void first_iter(Landscape& landscape, boost::fibers::barrier& bar, int start_row, int end_row, bool& is_dry, int step) {
    // std::cout << "I am here in first iter\n";
    for(int i = start_row; i < end_row; i++) {
        for(int j = 0; j < dim; j++) {
            // Receive raindrops
            if(step <= raining_time) {
                landscape.receive_rain_drop(i, j);
            }
            // Absorb
            landscape.absorb(i, j, absorption_rate);
            is_dry = is_dry && (landscape.get_raindrops(i, j) == 0);
            // Calculate trickled drops
            landscape.calculate_trickled_drops(i, j);
        }
    }
    bar.wait();
    // std::cout << "I am done in first iter\n";
}

void second_iter(Landscape& landscape, boost::fibers::barrier& bar, int start_row, int end_row, int col) {
    // std::cout << "I am here in second iter\n";
    for(int i = start_row; i < end_row; i++) {
        for(int j = 0; j < col; j++) {
            landscape.trickle_to(i, j);
        }
    }
    bar.wait();
    // std::cout << "I am done in second iter\n";
}



// void init_threadpool(int num_threads) {
//     boost::asio::io_service io_service;
//     boost::thread_group threadpool;
//     boost::asio::io_service::work work(io_service);

//     for(int i = 0; i < num_threads; i++) {
//         threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
//     }
// }

// void init_dry_status(bool* dry_status, int num_threads) {
//     dry_status = new bool[num_threads]();
//     for(int i = 0; i < num_threads; i++) {
//         dry_status[i] = true; // true means dry, false means not dry
//     }
// }

bool is_all_dry(const bool* dry_status, int num_threads) {
    bool ans = true;
    for(int i = 0; i < num_threads; i++) {
        // std::cout << i << ": " << dry_status[i] << std::endl;
        ans = ans && dry_status[i];
    }
    return ans;
}



int main(int argc, char* argv[]) {
    // std::cout << "start!\n";
    validate_arguments(argc, argv);
    num_threads = atoi(argv[1]);
    // std::cout << num_threads << std::endl;
    raining_time = atoi(argv[2]);
    absorption_rate = atof(argv[3]);
    dim = atoi(argv[4]);

    // init_threadpool
    boost::asio::io_service service;
    boost::thread_group threadpool;
    boost::asio::io_service::work work(service);
    boost::fibers::barrier bar(num_threads + 1);

    for(int i = 0; i < num_threads; i++) {
        threadpool.create_thread(boost::bind(&boost::asio::io_service::run, &service));
    }

    // std::cout << "*********************thread_pool inited\n";

    Landscape landscape(dim, argv[5]);
    int step = 1;
    int curr_dim = landscape.get_dim();
    int k = curr_dim / num_threads / 2; // evenly distributed
    bool* dry_status = new bool[num_threads]();
    // init_dry_status(dry_status, num_threads);

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    while(true) {
        // landscape.reset_is_dry();
        // Within one time step
        // First iteration
        for(int i = 0; i < num_threads; i++) {
            dry_status[i] = true;
            service.post(boost::bind(first_iter, boost::ref(landscape), boost::ref(bar), 2 * i * k, (2 * i + 1) * k, boost::ref(dry_status[i]), step));
            // first_iter(landscape, 2 * i * k, (2 * i + 1) * k, curr_dim, step, raining_time, absorption_rate);
        }

        // std::cout << "first loop done\n";
        bar.wait();

        for(int i = 0; i < num_threads; i++) {
            service.post(boost::bind(first_iter, boost::ref(landscape), boost::ref(bar), (2 * i + 1) * k, (2 * i + 2) * k, boost::ref(dry_status[i]), step));
            // first_iter(landscape, (2 * i + 1) * k, (2 * i + 2) * k, curr_dim, step, raining_time, absorption_rate);
        }
        // std::cout << "second loop done\n";
        bar.wait();
        if(is_all_dry(dry_status, num_threads)) {
            break;
        }

        // std::cout << "****************************** raindrops 1 ***************************\n";
        // landscape.print_raindrops();
        // std::cout << "****************************** absorb 1 *******************************\n";
        // landscape.print_absorbed_drops();
        // std::cout << "****************************** trickle 1 ******************************\n";
        // landscape.print_trickled_drops();

        // barrier
        // Second iteration
        for(int i = 0; i < num_threads; i++) {
            service.post(boost::bind(second_iter, boost::ref(landscape), boost::ref(bar), (2 * i) * k, (2 * i + 2) * k, curr_dim));
            // second_iter(landscape, 2 * i * k, (2 * i + 2) * k, curr_dim);
        }
        // std::cout << "third loop done\n";
        bar.wait();
        ++step;
        // std::cout << "****************************** raindrops 2 ***************************\n";
        // landscape.print_raindrops();
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double runtime = calc_time(start_time, end_time) / 1000000000.0;
    // threadpool.join_all();
    service.stop();
    delete[] dry_status;

    output_results(step, runtime, landscape);
    
    return EXIT_SUCCESS;
}