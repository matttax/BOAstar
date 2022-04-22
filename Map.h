#ifndef BOASTAR_MAP_H
#define BOASTAR_MAP_H

#include <iostream>
#include <fstream>
#include <string>

class Map {
private:
    int height, width;
    int start_x, start_y;
    int finish_x, finish_y;
    double **grid;
    void read_map(std::string file);

public:
    Map();
    ~Map();
    int get_height() const;
    int get_width() const;
    int get_start_x() const;
    int get_start_y() const;
    int get_finish_x() const;
    int get_finish_y() const;
    double get_cell(int i, int j);
    bool is_walkable(int i, int j);
};


#endif //BOASTAR_MAP_H
