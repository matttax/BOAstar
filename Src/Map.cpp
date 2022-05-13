#include "Map.h"

Map::Map() {
    height = -1;
    width = -1;
    start_y = -1;
    start_x = -1;
    finish_y = -1;
    finish_x = -1;
    grid = nullptr;
    is_biobjective = true;
    allow_diagonal = true;
    allow_squeeze = true;
    allow_cut_corners = true;
    heuristic = EUCLID;
    visualize = false;
    logfile = "noname";
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

int Map::get_start_y() const {
    return start_y;
}

int Map::get_start_x() const {
    return start_x;
}

int Map::get_finish_y() const {
    return finish_y;
}

int Map::get_finish_x() const {
    return finish_x;
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
         grid_open = false, grid_close = false;
    int current_row = 0, current_col = 0;
    std::string name;
    size_t open_index, close_index;
    while (std::getline(infile, line)) {
        open_index = line.find('<');
        close_index = line.find('>');
        if (open_index == SIZE_MAX || close_index == SIZE_MAX && !line.empty()) {
            std::cout << "\"" << line << "\" - invalid format.\n";
            return false;
        }
        name = std::string(line.begin() + open_index + 1, line.begin() + close_index);
        if (!map_open) {
            if (name == "map") {
                map_open = true;
                continue;
            } else {
                std::cout << "Map attribute not found.";
                return false;
            }
        } else if (name == "/map") {
            map_closed = true;
            break;
        } else if (width == -1 || height == -1) {
            line.erase(line.begin(), line.begin() + close_index + 1);
            if (name == "width") {
                if (!get_number(width, line, name))
                    return false;
            } else if (name == "height") {
                if (!get_number(height, line, name))
                    return false;
            } else {
                std::cout << "Width and height tags should precede all others.";
                return false;
            }
        } else if (!grid_open) {
            if (name == "grid") {
                grid_open = true;
                grid = new double *[height];
                for (int i = 0; i < height; ++i)
                    grid[i] = new double[width]{};
                continue;
            }
        } else if (name == "row") {
            line.erase(line.begin(), line.begin() + close_index + 1);
            open_index = line.find('<');
            close_index = line.find('>');
            if (open_index != SIZE_MAX && close_index != SIZE_MAX &&
                    std::string(line.begin() + open_index + 1, line.begin() + close_index) == "/row") {
                std::istringstream row(std::string(line.begin(), line.begin() + open_index));
                while (current_col < width && row >> grid[current_row][current_col]) {
                    if (grid[current_row][current_col] > 1 || grid[current_row][current_col] < 0)
                        grid[current_row][current_col] = 1;
                    current_col++;
                }
                current_col = 0;
                current_row++;
            } else {
                std::cout << "Row isn't closed.\n";
                return false;
            }
        } else if (name == "/grid") {
            grid_close = true;
        } else if (!grid_close) {
            std::cout << "Grid tag isn't closed";
            return false;
        }
        else if (start_y == -1 || start_x == -1 || finish_y == -1 || finish_x == -1) {
            line.erase(line.begin(), line.begin() + close_index + 1);
            if (name == "starty") {
                if (!get_number(start_y, line, name))
                    return false;
            } else if (name == "startx") {
                if (!get_number(start_x, line, name))
                    return false;
            } else if (name == "finishy") {
                if (!get_number(finish_y, line, name))
                    return false;
            } else if (name == "finishx") {
                if (!get_number(finish_x, line, name))
                    return false;
            } else return false;
        } else if (name == "allowdiagonal" || name == "cutcorners" || name == "allowsqueeze" || name == "biobjective" || name == "visualize") {
            line.erase(line.begin(), line.begin() + close_index + 1);
            if (name == "allowdiagonal")
                if (!get_boolean(allow_diagonal, line, name))
                    return false;
            if (name == "allowsqueeze")
                if (!get_boolean(allow_squeeze, line, name))
                    return false;
            if (name == "cutcorners")
                if (!get_boolean(allow_cut_corners, line, name))
                    return false;
            if (name == "biobjective")
                if (!get_boolean(is_biobjective, line, name))
                    return false;
            if (name == "visualize")
                if (!get_boolean(visualize, line, name))
                    return false;
        } else if (name == "logfile") {
            line.erase(line.begin(), line.begin() + close_index + 1);
            if (!get_string(logfile, name))
                return false;
        } else if (name == "heuristic") {
            std::string h;
            if (!get_string(h, name))
                return false;
            if (h == "euclid")
                heuristic = EUCLID;
            else if (h == "manhattan")
                heuristic = MANHATTAN;
            else if (h == "chebyshev")
                heuristic = CHEBYSHEV;
            else if (h == "octile")
                heuristic = OCTILE;
            else return false;
        } else return false;
    }
    if (!is_biobjective)
        reformat_map();
    if (width > 60 || height > 60) {
        visualize = false;
        std::cout << "Map is too large for visualization.\n";
    }
    if (!map_closed) {
        std::cout << "Map tag isn't closed.\n";
        return false;
    }
    if (!grid_open) {
        std::cout << "Grid isn't found.\n";
        return false;
    }
    if (width <= 0 || height <= 0) {
        std::cout << "Width and height must be positive.\n";
        return false;
    }
    if (start_y < 0 || start_y >= height || start_x < 0 || start_x >= width || grid[start_y][start_x] == 1) {
        std::cout << "Start point must be traversable.\n";
        return false;
    }
    if (finish_y < 0 || finish_y >= height || finish_x < 0 && finish_x >= width || grid[finish_y][finish_x] == 1) {
        std::cout << "Finish point must be traversable.\n";
        return false;
    }
    std::cout << "Map read.\n";
    return true;
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
                                grid[mov.first + i][mov.second + j] = grid[i][j] - 0.1 >=0 ? grid[i][j] - 0.1 : 0;
                }
            }
        }
    }
}

Heuristic Map::get_heuristic() {
    return heuristic;
}

bool Map::get_number(int &number, std::string line, const std::string &tag) {
    size_t open_index = line.find('<'),
            close_index = line.find('>');
    if (open_index != SIZE_MAX && close_index != SIZE_MAX &&
            std::string(line.begin() + open_index + 1, line.begin() + close_index) == "/" + tag) {
        try {
            number = std::stoi(std::string(line.begin(), line.begin() + open_index));
        } catch(std::exception &err) {
            std::cout << "Invalid " << tag << "\n";
            return false;
        }
    } else {
        std::cout << "Tag " << tag << "isn't closed.\n";
        return false;
    }
    return true;
}

bool Map::get_boolean(bool &boolean, std::string line, const std::string &tag) {
    boolean = false;
    size_t open_index = line.find('<'),
            close_index = line.find('>');
    if (open_index != SIZE_MAX && close_index != SIZE_MAX &&
            std::string(line.begin() + open_index + 1, line.begin() + close_index) == "/" + tag) {
        boolean = std::string(line.begin(), line.begin() + open_index) == "true" ||
                  std::string(line.begin(), line.begin() + open_index) == "1";
    } else {
        std::cout << "Tag " + tag << " isn't closed.\n";
        return false;
    }
    return true;
}

bool Map::squeeze_allowed() const {
    return allow_squeeze;
}

bool Map::diagonal_allowed() const {
    return allow_diagonal;
}

bool Map::corners_allowed() const {
    return allow_cut_corners;
}

bool Map::get_string(std::string &line, const std::string &tag) {
    size_t open_index = line.find('<'),
            close_index = line.find('>');
    if (open_index != SIZE_MAX && close_index != SIZE_MAX &&
            std::string(line.begin() + open_index + 1, line.begin() + close_index) == "/" + tag) {
        line = std::string(line.begin(), line.begin() + open_index);
    } else {
        std::cout << "Tag " + tag << " isn't closed.\n";
        return false;
    }
    return true;
}

bool Map::do_visualize() const {
    return visualize;
}

std::string Map::get_logfile_name() const {
    return logfile;
}
