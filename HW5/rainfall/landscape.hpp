#include <algorithm>
#include <vector>
#include <utility>

#include <iostream>

class Landscape {
    private:
        int dim;
        double* raindrops;
        double* absorbed_drops;
        double* trickled_drops;
        std::vector<std::pair<int, int> >* trickling_directions;

        void calculate_trickling_directions(int row, int col, double* elevations);

        // Debug purpose
        void print_elevations(double* elevations);

    public:
        Landscape(int _dim, const char* filepath);

        ~Landscape();

        int get_dim(void) {
            return dim;
        }

        void receive_rain_drop(int row, int col) {
            raindrops[row * dim + col]++;
        }

        void absorb(int row, int col, double absorption_rate);

        void trickle(int row, int col);

        void calculate_trickled_drops(int row, int col);

        // Debug purpose
        void insert(void);
        void print_trickling_directions(void);
        void print_raindrops(void);
        void print_trickled_drops(void);
};