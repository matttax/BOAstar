#include <iostream>
#include <chrono>
#include "Search.h"

int main() {
    std::string file_name = "/home/matttax/Документы/test6";
    Search search(file_name);
    std::ofstream outfile(file_name + "_log");
    auto start_time = std::chrono::steady_clock::now();
    auto sol = search.boa_star();
    auto end_time = std::chrono::steady_clock::now();
    std::cout << "Time: " << std::chrono::duration<double>(end_time - start_time).count()
              << "\nSolutions found: " << sol.size() << "\n";

    outfile << "<root>\n";
    search.print_map(outfile);
    outfile << "\t<solutions>\n";
    for (auto & s : sol) {
        search.print_solution(s, outfile);
    }
    outfile << "\t</solutions>";
    search.print_opened(outfile);
    outfile << "</root>";
    return 0;
}
