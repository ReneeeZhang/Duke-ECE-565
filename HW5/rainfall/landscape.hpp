#include <algorithm>
#include <vector>
#include <utility>

#include <iostream>

class Landscape {
    private:
        int dim;
        // std::vector<std::vector<double> > elevations;
        // std::vector<std::vector<double> > raindrops;
        // std::vector<std::vector<double> > absorbed_drops;
        // std::vector<std::vector<std::unordered_set<Direction> > > trickling_direction;
        
        double* raindrops;
        double* absorbed_drops;
        std::vector<std::pair<int, int> >* trickling_directions;
        
        void receive_rain_drop(int row, int col) {
            raindrops[row * dim + col]++;
        }

        void absorb(int row, int col, double absorption_rate);

        void calculate_trickling_directions(int row, int col, double* elevations);

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
        void print_trickling_directions(void);
};