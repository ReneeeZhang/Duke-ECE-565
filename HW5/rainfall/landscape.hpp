#include <algorithm>
#include <unordered_set>

#include <iostream>

enum class Direction {North, East, South, West};
class Landscape {
    private:
        int dim;
        // std::vector<std::vector<double> > elevations;
        // std::vector<std::vector<double> > raindrops;
        // std::vector<std::vector<double> > absorbed_drops;
        // std::vector<std::vector<std::unordered_set<Direction> > > trickling_direction;
        
        double* raindrops;
        double* absorbed_drops;
        std::unordered_set<Direction>* trickling_directions;
        
        void receive_rain_drop(int row, int col) {
            raindrops[row * dim + col]++;
        }

        void absorb(int row, int col, double absorption_rate);

        void calculate_trickling_drops(int row, int col);

        void trickle(int row, int col);

        // Debug purpose
        void print_elevations(double* elevations);

    public:
        Landscape(int _dim, const char* filepath);

        ~Landscape() {
            delete[] raindrops;
            delete[] absorbed_drops;
            delete[] trickling_directions;
        }

        // Debug purpose
        void insert(void);
};