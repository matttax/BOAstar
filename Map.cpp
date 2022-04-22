#include "Map.h"

Map::~Map() {
    if (grid) {
        for (int i = 0; i < height; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
    }
}

Map::Map() {
    height = 0;
    width = 0;
    start_x = -1;
    start_y = -1;
    finish_x = -1;
    finish_y = -1;
    grid = nullptr;
}

int Map::get_height() const {
    return height;
}

int Map::get_width() const {
    return width;
}

int Map::get_start_x() const {
    return start_x;
}

int Map::get_start_y() const {
    return start_y;
}

int Map::get_finish_x() const {
    return finish_x;
}

int Map::get_finish_y() const {
    return finish_y;
}

double Map::get_cell(int i, int j) {
    if (i < 0 || i >= height || j < 0 || j >= width)
        return -1;
    if (grid)
        return grid[i][j];
    return -1;
}

bool Map::is_walkable(int i, int j) {
    if (i < 0 || i >= height || j < 0 || j >= width)
        return false;
    if (grid)
        if (grid[i][j] != 1)
            return true;
    return false;
}

void Map::read_map(std::string file) {
    std::ifstream infile(file);
    infile >> height >> width;
    grid = new double *[height];
    for (int i = 0; i < height; ++i) {
        grid[i] = new double[width];
        for (int j = 0; j < width; ++j)
            infile >> grid[i][j];
    }
    infile >> start_x >> start_y;
    infile >> finish_x >> finish_y;
}


