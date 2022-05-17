#include <iostream>
#include <fstream>
#include <random>

int random_cell() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 1);
    return dist(mt);
}

int main() {
    for (int N = 10; N <= 500; N += 10) {
        std::ofstream outfile("/SerialTests/test" + std::to_string(N) + ".xml");
        outfile << "<map>\n\t<width>" << N << "</width>\n\t<height>" << N << "</height>\n\t<grid>\n";
        for (int i = 0; i < N; ++i) {
            outfile << "\t\t<row>";
            for (int j = 0; j < N; ++j) {
                if (i == j && i > 0 && j > 0)
                    outfile << "1 ";
                else outfile << "0 ";
            }
            outfile << "</row>\n";
        }
        outfile << "\t</grid>\n\t<startx>0</startx>\n\t<starty>0</starty>\n\t<finishx>" << N-1 << "</finishx>\n\t<finishy>" << N/2 << "</finishy>\n"
                    "\t<biobjective>false</biobjective>\n</map>";
    }
}
