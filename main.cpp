#include <iostream>
#include "Search.h"

int main() {
    Search search("/home/matttax/Документы/test2");
    auto sol = search.boa_star();
    std::cout << sol.size() << "\n";
    for (auto & s : sol) {
        std::cout << s->f_length << " " << s->f_safety << "\n";
        search.print_solution(s);
    }
    return 0;
}
