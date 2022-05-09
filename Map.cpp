#include "Map.h"

Map::Map() {
    height = 0;
    width = 0;
    start_x = -1;
    start_y = -1;
    finish_x = -1;
    finish_y = -1;
    grid = nullptr;
    is_biobjective = false;
    allow_diagonal = true;
    allow_squeeze = true;
    allow_cut_corners = true;
    heuristic = EUCLID;
}

Map::~Map() {
    if (grid) {
        for (int i = 0; i < height; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
    }
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

bool Map::read_map(const std::string& filename) {
    std::ifstream infile(filename);
    std::string line;
    bool map_open = false, map_closed = false,
            width_present = false,
            height_present = false,
            grid_open = false, grid_close = false,
            startx_present = false, starty_present = false,
            finishx_present = false, finishy_present = false;
    int current_row = 0, current_col = 0;
    while(std::getline(infile, line)) {
        size_t open_index = line.find('<'),
                close_index = line.find('>');
        std::string name = std::string(line.begin() + open_index + 1, line.begin() + close_index);
        if (!map_open) {
            if (name == "map") {
                map_open = true;
                continue;
            } else return false;
        } else if (name == "/map") {
            break;
        } else if (!width_present || !height_present) {
            line.erase(line.begin(), line.begin() + close_index + 1);
            if (name == "width") {
                width = get_number(line, "/width", width_present);
            } else if (name == "height") {
                height = get_number(line, "/height", height_present);
            } else return false;
        } else if (!grid_open) {
            if (name == "grid") {
                grid_open = true;
                grid = new double *[height];
                for (int i = 0; i < height; ++i)
                    grid[i] = new double [width]{};
                continue;
            }
        } else if (name == "row") {
            line.erase(line.begin(), line.begin() + close_index + 1);
            open_index = line.find('<');
            close_index = line.find('>');
            if (std::string(line.begin() + open_index + 1, line.begin() + close_index) == "/row") {
                std::istringstream row(std::string(line.begin(), line.begin() + open_index));
                while (current_col < width && row >> grid[current_row][current_col]) {
                    current_col++;
                }
                current_col = 0;
                current_row++;
            }
        } else if (name == "/grid") {
            grid_close = true;
        } else if (!grid_close)
            return false;
        else if (!startx_present || !starty_present || !finishx_present || !finishy_present) {
            line.erase(line.begin(), line.begin() + close_index + 1);
            if (name == "startx")
                start_x = get_number(line, "/startx", starty_present);
            else if (name == "starty")
                start_y = get_number(line, "/starty", startx_present);
            else if (name == "finishx")
                finish_x = get_number(line, "/finishx", finishx_present);
            else if (name == "finishy")
                finish_y = get_number(line, "/finishy", finishy_present);
            else return false;
        } else if (name == "allowdiagonal" || name == "cutcorners" || name == "allowsqueeze" || name == "biobjective") {
            line.erase(line.begin(), line.begin() + close_index + 1);
            if (name == "allowdiagonal")
                allow_diagonal = get_boolean(line, "/allowdiagonal");
            if (name == "allowsqueeze")
                allow_squeeze = get_boolean(line, "/allowsqueeze");
            if (name == "cutcorners")
                allow_cut_corners = get_boolean(line, "/cutcorners");
            if (name == "biobjective")
                is_biobjective = get_boolean(line, "/biobjective");
        } else return false;
    }
    if (is_biobjective)
        reformat_map();
    return grid_open &&
           startx_present && starty_present &&
           finishx_present && finishy_present &&
           width_present && height_present &&
           start_x >= 0 && start_x <= width &&
           start_y >= 0 && start_y <= height &&
           finish_x >= 0 && finish_x <= width &&
           finish_y >= 0 && finish_y <= height &&
           grid[start_x][start_y] != 1 && grid[finish_x][finish_y] != 1;
}

void Map::reformat_map() {
    std::vector<std::pair<int, int>> moves = {{0, 1}, {1, 0}, {0, -1}, {-1, 0},
                                              {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    for (double k = 1.0; k > 0; k -= 0.1) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (grid[i][j] == k) {
                    for (auto mov : moves)
                        if (mov.first + i >= 0 && mov.first + i < height && mov.second + j >= 0 && mov.second + j < width)
                            if (grid[mov.first + i][mov.second + j] < grid[i][j])
                                grid[mov.first + i][mov.second + j] = grid[i][j] - 0.1;
                }
            }
        }
    }
}

Heuristic Map::get_heuristic() {
    return heuristic;
}

int Map::get_number(std::string line, const std::string &attribute, bool &present) {
    int num = -1;
    size_t open_index = line.find('<'),
            close_index = line.find('>');
    if (std::string(line.begin() + open_index + 1, line.begin() + close_index) == attribute) {
        num = std::stoi(std::string(line.begin(), line.begin() + open_index));
        present = true;
    }
    return num;
}

bool Map::get_boolean(std::string line, const std::string &attribute) {
    bool boolean = false;
    size_t open_index = line.find('<'),
            close_index = line.find('>');
    if (std::string(line.begin() + open_index + 1, line.begin() + close_index) == attribute) {
        boolean = std::string(line.begin(), line.begin() + open_index) == "true" ||
                  std::string(line.begin(), line.begin() + open_index) == "1";
    }
    return boolean;
}