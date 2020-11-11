#include <fstream>
#include <sstream>
#include <string>
#include <iostream> // Debug purpose

#include "landscape.hpp"

Landscape::Landscape(int _dim, const char* filepath) {
    dim = _dim;
    points.resize(dim);
    for(int i = 0; i < dim; ++i) {
        points[i].resize(dim, 0);
    }
    std::ifstream input_file(filepath, std::ifstream::in);
    std::string line;
    int row = 0;
    while(std::getline(input_file, line)) {
        std::stringstream ss(line);
        int col;
        for(col = 0; col < dim; ++col) {
            ss >> points[row][col];
        }
        ++row;
    }
}

// Debug purpose
void Landscape::print(void) {
    for(int i = 0; i < dim; ++i) {
        for(int j = 0; j < dim; ++j) {
            std::cout << points[i][j] << ' ';
        }
        std::cout << std::endl;
    }
} 