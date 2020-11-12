#include <fstream>
#include <sstream>
#include <string>
#include <iostream> // Debug purpose

#include "landscape.hpp"

Landscape::Landscape(int _dim, const char* filepath): dim(_dim),
                                                      raindrops(new double[dim * dim]),
                                                      absorbed_drops(new double[dim * dim]),
                                                      trickling_directions(new std::vector<std::pair<int, int> >[dim * dim]) {
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
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            calculate_trickling_drops(i, j, elevations);
        }
    }
    delete[] elevations;
}

void Landscape::calculate_trickling_drops(int row, int col, double* elevations) {
    int startx = std::max(0, col - 1);
    int endx = std::min(dim - 1, col + 1);
    int starty = std::max(0, row - 1);
    int endy = std::min(dim - 1, row + 1);

    for(int i = starty; i <= endy; ++i) {
        for(int j = startx; j <= endx; ++j) {
            if((i == row && j == col) || std::abs(i - row) + std::abs(j - col) == 2) {
                continue;
            }
            if(elevations[row * dim + col] > elevations[i * dim + j]) {
                trickling_directions[row * dim + col].emplace_back(std::pair<int, int>(i, j));
            }
        }
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
    trickling_directions[0].emplace_back(std::pair<int, int>(1, 1));
    std::cout << "insert!\n";
    for(auto& it : trickling_directions[0]) {
        std::cout << it.first << ", " << it.second << std::endl;
    }
}

void Landscape::print_trickling_directions(void) {
    for(int i = 0; i < dim; ++i) {
        for(int j = 0; j < dim; ++j) {
            if(trickling_directions[i * dim + j].empty()) {
                std::cout << "null ";
            } else {
                std::cout << '[';
                for(size_t k = 0; k < trickling_directions[i * dim + j].size(); ++k) {
                    std::cout << "(" << trickling_directions[i * dim + j][k].first << ", " << trickling_directions[i * dim + j][k].second << ")";
                }
                std::cout << "] ";
            }
        }
        std::cout << std::endl;
    }
}