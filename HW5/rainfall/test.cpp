#include "landscape.hpp"

void test_trickling_directions(void) {
    Landscape ls(4, "../test/input/sample_4x4.in");
    ls.print_trickling_directions();
}

void test_raindrops(void) {
    Landscape ls(4, "../test/input/sample_4x4.in");
    for(int i = 0; i < ls.get_dim(); i++) {
        for(int j = 0; j < ls.get_dim(); j++) {
            ls.receive_rain_drop(i, j);
        }
    }
    ls.print_raindrops();

    for(int i = 0; i < ls.get_dim(); i++) {
        for(int j = 0; j < ls.get_dim(); j++) {
            ls.absorb(i, j, 0.2);
        }
    }
    ls.print_raindrops();
    ls.print_trickled_drops();
    for(int i = 0; i < ls.get_dim(); i++) {
        for(int j = 0; j < ls.get_dim(); j++) {
            ls.calculate_trickled_drops(i, j);
        }
    }
    ls.print_trickled_drops();    

    for(int i = 0; i < ls.get_dim(); i++) {
        for(int j = 0; j < ls.get_dim(); j++) {
            ls.trickle_to(i, j);
        }
    }
    ls.print_raindrops();
}

int main(void) {
    std::cout << "@@@@@@@@@@@ test trickling directions @@@@@@@@@@@@\n";
    test_trickling_directions();
    std::cout << "@@@@@@@@@@@ test raindrops @@@@@@@@@@@@\n";
    test_raindrops();
}