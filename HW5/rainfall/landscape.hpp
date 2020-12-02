#include <algorithm>
#include <vector>
#include <utility>

#include <iostream>

class Landscape {
    private:
        int dim;
        bool is_dry;
        double* raindrops;
        double* absorbed_drops;
        double* trickled_drops;
        std::vector<std::pair<int, int> >* trickling_directions;

        void calculate_trickling_directions(int row, int col, double* elevations);

        // Debug purpose
        void print_elevations(double* elevations) const;

    public:
        Landscape(int _dim, const char* filepath);

        ~Landscape();

        int get_dim(void) const {
            return dim;
        }

        double get_raindrops(int row, int col) {
            return raindrops[row * dim + col];
        }

        void reset_is_dry(void) {
            is_dry = true;
        }

        bool has_been_dry(void) const {
            return is_dry;
        }

        void receive_rain_drop(int row, int col);

        void absorb(int row, int col, double absorption_rate);

        void absorb_pt(int row, int col, double absorption_rate);

        void trickle_to(int row, int col);

        void calculate_trickled_drops(int row, int col);

        // Debug purpose
        void insert(void);
        void print_trickling_directions(void) const;
        void print_raindrops(void) const;
        void print_absorbed_drops(void) const;
        void print_trickled_drops(void) const;
};