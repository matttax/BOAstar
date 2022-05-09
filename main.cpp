#include <iostream>
#include <chrono>
#include "Search.h"

int main() {
    std::string file_name = "/home/matttax/Документы/test1";
    Search search(file_name);
    std::ofstream outfile(file_name + "_log");
    auto start_time = std::chrono::steady_clock::now();
    auto sol = search.boa_star();
    auto end_time = std::chrono::steady_clock::now();
    std::cout << "Time: " << std::chrono::duration<double>(end_time - start_time).count()
              << "\nSolutions found: " << sol.size() << "\n";
    outfile << "<solutions>\n";
    for (auto & s : sol) {
        outfile << "\t<solution>\n";
        outfile << "\t\t<length>" << s->f_length << "</length>\n";
        outfile << "\t\t<safety>" << s->f_safety << "</safety>\n";
        outfile << "\t\t<path>\n";
        search.print_solution(s, outfile);
        outfile << "\t\t</path>\n";
        outfile << "\t</solution>\n";
    }
    outfile << "</solutions>";
    return 0;
}
