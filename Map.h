#ifndef BOASTAR_MAP_H
#define BOASTAR_MAP_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

enum Heuristic {
    EUCLID,
    MANHATTAN,
    CHEBYSHEV,
    OCTILE
};

class Map {
private:
    int height, width;
    int start_x, start_y;
    int finish_x, finish_y;
    Heuristic heuristic;

    double **grid;

    int get_number(std::string line, const std::string &attribute, bool &present);
    bool get_boolean(std::string line, const std::string &attribute);

public:
    Map();
    ~Map();
    bool is_biobjective;
    bool allow_squeeze, allow_diagonal, allow_cut_corners;
    int get_height() const;
    int get_width() const;
    int get_start_x() const;
    int get_start_y() const;
    int get_finish_x() const;
    int get_finish_y() const;
    double get_cell(int i, int j);
    bool is_walkable(int i, int j);
    bool read_map(const std::string& filename);
    Heuristic get_heuristic();
    void reformat_map();
};


#endif //BOASTAR_MAP_H
