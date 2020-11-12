#include <fstream>
#include <sstream>
#include <string>
#include <iostream> // Debug purpose

#include "landscape.hpp"

Landscape::Landscape(int _dim, const char* filepath): dim(_dim),
                                                      raindrops(new double[dim * dim]),
                                                      absorbed_drops(new double[dim * dim]),
                                                      trickling_directions(new std::unordered_set<Direction>[dim * dim]) {
    // elevations = std::vector<std::vector<double> > (dim);
    // raindrops = std::vector<std::vector<double> > (dim);
    // absorbed_drops = std::vector<std::vector<double> > (dim);
    // trickling_direction = std::vector<std::vector<std::unordered_set<Direction> > > (dim);
    // for(int i = 0; i < dim; ++i) {
    //     elevations[i] = std::vector<double> (dim, 0);
    //     raindrops[i] = std::vector<double> (dim, 0);
    //     absorbed_drops[i] = std::vector<double> (dim, 0);
    // }
    double* elevations = new double[dim * dim];
    std::ifstream input_file(filepath, std::ifstream::in);
    std::string line;
    int row = 0;
    while(std::getline(input_file, line)) {
        std::stringstream ss(line);
        int col;
        for(col = 0; col < dim; ++col) {
            ss >> elevations[row * dim + col];
        }
        ++row;
    }
    print_elevations(elevations);
    delete[] elevations;
}

void Landscape::calculate_trickling_drops(int row, int col) {
    // 正向思考 搞个数组存放东西南北？
    if(row - 1 >= 0) {

    }
}

void Landscape::absorb(int row, int col, double absorption_rate) {
    absorbed_drops[row * dim + col] += std::max(0.0, absorption_rate);
    raindrops[row * dim + col] = std::max(0.0, raindrops[row * dim + col] - absorption_rate);
}

// Debug purpose
void Landscape::print_elevations(double* elevations) {
    for(int i = 0; i < dim; ++i) {
        for(int j = 0; j < dim; ++j) {
            std::cout << elevations[i * dim + j] << ' ';
        }
        std::cout << std::endl;
    }
}

void Landscape::insert(void) {
    trickling_directions[0].insert(Direction::East);
    std::cout << "insert!\n";
    for(auto& it : trickling_directions[0]) {
        if(it == Direction::East){
            std::cout << "correct" << std::endl;
        }
    }
}