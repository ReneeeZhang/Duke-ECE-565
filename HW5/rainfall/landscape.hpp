#include <vector>

class Landscape {
    private:
        int dim;
        std::vector<std::vector<double> > points;

    public:
        Landscape(int _dim, const char* filepath);
        void print(void); // Debug purpose
};