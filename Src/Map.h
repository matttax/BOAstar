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
    int start_y, start_x;
    int finish_y, finish_x;
    bool visualize;
    bool is_biobjective;
    bool allow_squeeze, allow_diagonal, allow_cut_corners;
    Heuristic heuristic;
    double **grid;
    std::string logfile;
    bool get_number(int &number, std::string line, const std::string &tag);
    bool get_boolean(bool &boolean, std::string line, const std::string &tag);
    bool get_string(std::string &line, const std::string &tag);

public:
    Map();
    ~Map();
    int get_height() const;
    int get_width() const;
    int get_start_y() const;
    int get_start_x() const;
    int get_finish_y() const;
    int get_finish_x() const;
    bool squeeze_allowed() const;
    bool diagonal_allowed() const;
    bool corners_allowed() const;
    bool do_visualize() const;
    std::string get_logfile_name() const;
    double get_cell(int i, int j);
    bool is_walkable(int i, int j);
    bool read_map(const std::string& filename);
    Heuristic get_heuristic();
    void reformat_map();
};


#endif //BOASTAR_MAP_H
