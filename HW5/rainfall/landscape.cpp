#include <fstream>
#include <sstream>
#include <string>

// Debug purpose
#include <iostream> 
#include <iomanip>

#include "landscape.hpp"

Landscape::Landscape(int _dim, const char* filepath): dim(_dim),
                                                      is_dry(true),
                                                      raindrops(new double[dim * dim]()),
                                                      absorbed_drops(new double[dim * dim]()),
                                                      trickled_drops(new double[dim* dim]()),
                                                      trickling_directions(new std::vector<std::pair<int, int> >[dim * dim]()) {
    
    double* elevations = new double[dim * dim]();
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
    // print_elevations(elevations);
    for(int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            calculate_trickling_directions(i, j, elevations);
        }
    }
    delete[] elevations;
}

Landscape::~Landscape() {
    delete[] raindrops;
    delete[] absorbed_drops;
    delete[] trickled_drops;
    delete[] trickling_directions;
}

void Landscape::calculate_trickling_directions(int row, int col, double* elevations) {
    int startx = std::max(0, col - 1);
    int endx = std::min(dim - 1, col + 1);
    int starty = std::max(0, row - 1);
    int endy = std::min(dim - 1, row + 1);

    for(int i = starty; i <= endy; ++i) {
        for(int j = startx; j <= endx; ++j) {
            if((i == row && j == col) || std::abs(i - row) + std::abs(j - col) == 2) {
                continue;
            }
            std::vector<std::pair<int, int> >& center_point_trickling_direction = trickling_directions[row * dim + col];
            if(elevations[row * dim + col] > elevations[i * dim + j]) {
                if(center_point_trickling_direction.empty() || elevations[i * dim + j] == elevations[center_point_trickling_direction.back().first * dim + center_point_trickling_direction.back().second]) {
                    center_point_trickling_direction.emplace_back(std::pair<int, int>(i, j));
                } else if(elevations[i * dim + j] < elevations[center_point_trickling_direction.back().first * dim + center_point_trickling_direction.back().second]) {
                    center_point_trickling_direction.pop_back();
                    center_point_trickling_direction.emplace_back(std::pair<int, int>(i, j));    
                }
            }
        }
    }
}

void Landscape::receive_rain_drop(int row, int col) {
    raindrops[row * dim + col]++;
}

void Landscape::absorb(int row, int col, double absorption_rate) {
    double num_absorbed = std::min(raindrops[row * dim + col], absorption_rate);
    absorbed_drops[row * dim + col] += num_absorbed;
    raindrops[row * dim + col] -= num_absorbed;
    is_dry = is_dry && (raindrops[row * dim + col] == 0);
}

void Landscape::trickle_to(int row, int col) {
    raindrops[row * dim + col] += trickled_drops[row * dim + col];
    trickled_drops[row * dim + col] = 0;
}

void Landscape::calculate_trickled_drops(int row, int col) {
    if(trickling_directions[row * dim + col].empty()) {
        return;
    }
    for(auto& it : trickling_directions[row * dim + col]) {
        trickled_drops[it.first * dim + it.second] += std::min(1.0, raindrops[row * dim + col]) / trickling_directions[row * dim + col].size();
    }
    raindrops[row * dim + col] -= std::min(1.0, raindrops[row * dim + col]);
}

/***************************************************
 * Debug purpose
 * *************************************************/
void Landscape::print_elevations(double* elevations) const {
    for(int i = 0; i < dim; ++i) {
        for(int j = 0; j < dim; ++j) {
            std::cout << std::setw(8) << std::setprecision(6) << elevations[i * dim + j] << ' ';
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

void Landscape::print_trickling_directions(void) const {
    std::cout << "********* trickling directions ***********\n";
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

void Landscape::print_raindrops (void) const {
    for(int i = 0; i < dim; ++i) {
        for(int j = 0; j < dim; ++j) {
            std::cout << std::setw(8) << std::setprecision(6) << raindrops[i * dim + j] << ' ';
        }
        std::cout << std::endl;
    }
}

void Landscape::print_absorbed_drops (void) const {
    for(int i = 0; i < dim; ++i) {
        for(int j = 0; j < dim; ++j) {
            std::cout << std::setw(8) << std::setprecision(6) << absorbed_drops[i * dim + j] << ' ';
        }
        std::cout << std::endl;
    }
}

void Landscape::print_trickled_drops(void) const {
    std::cout << "********* trickle_drops ***********\n";
    for(int i = 0; i < dim; ++i) {
        for(int j = 0; j < dim; ++j) {
            std::cout << std::setw(8) << std::setprecision(6) << trickled_drops[i * dim + j] << ' ';
        }
        std::cout << std::endl;
    }
}