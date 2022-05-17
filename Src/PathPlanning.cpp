#include <iostream>
#include <chrono>
#include "Search.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "No arguments provided.";
        exit(-1);
    }
    std::string file_name = argv[1];
    std::string logfile_name;
    if (file_name.find(".xml")) {
        logfile_name = std::string(file_name.begin(), file_name.begin() + file_name.find(".xml")) + "_log.xml";
    } else {
        std::cout << "File must be XML.";
        exit(-1);
    }
    Search search(file_name);
    if (search.get_logfile() != "noname") {
        logfile_name = search.get_logfile();
    }
    std::ofstream outfile(logfile_name);
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
    outfile << "\t</solutions>\n";
    search.print_opened(outfile);
    outfile << "</root>";
    if (search.visualize()) {
        if (sol.size() == 0) {
            std::cout << "Nothing to visualize.";
        } else {
//            std::string command = "start PathVisualizer\\PathVisualizer.exe " + logfile_name;
//            system(command.c_str());
        }
    }
    return 0;
}
