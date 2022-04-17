#include <iostream>
#include "Search.h"

int main() {
    Search search;
    auto sol = search.boa_star();
    std::reverse(sol.begin(), sol.end());
    std::cout << sol.size() << "\n";
    for (int i = 0; i < sol.size(); ++i) {
        std::cout << sol[i]->f_length << " " << sol[i]->f_safety << "\n";
        //search.print_solution(sol[i]);
    }
    return 0;
}
