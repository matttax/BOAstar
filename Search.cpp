#include <iomanip>
#include "Search.h"

Search::Search(const std::string& file) {
    if(!map.read_map(file))
        exit(-1);
}

std::vector<Node *> Search::boa_star() {
    std::vector<Node*> solutions;
    Node *start = new Node(map.get_start_x(), map.get_start_y(), 0, map.get_cell(map.get_start_x(), map.get_start_y()),
                           get_hvalue(map.get_start_x(), map.get_start_y()), nullptr);
    Open open;
    open.add(start);
    while (!open.empty()) {
        Node *current = open.top();
        if (current->g_safety >= get_gsafety_min(current->i, current->j) ||
            current->f_safety >= get_gsafety_min(map.get_finish_x(), map.get_finish_y())) {
            continue;
        }
        gsafety_min[std::make_pair(current->i, current->j)] = current->g_safety;
        if (current->i == map.get_finish_x() && current->j == map.get_finish_y()) {
            solutions.push_back(current);
            continue;
        }
        auto children = get_children(current->i, current->j);
        for (auto child : children) {
            double length = (std::abs(current->i - child.first) + std::abs(current->j - child.second) == 2) ? std::sqrt(2) : 1.0,
                   safety = map.get_cell(child.first, child.second) + 0.1 * length;
            Node *child_node = new Node(child.first, child.second, length, safety,
                                       get_hvalue(child.first, child.second), current);
            if (child_node->g_safety >= get_gsafety_min(child.first, child.second) ||
                child_node->f_safety >= get_gsafety_min(map.get_finish_x(), map.get_finish_y())) {
                continue;
            }
            open.add(child_node);
        }
    }
    return solutions;
}

std::vector<std::pair<int, int>> Search::get_children(int i, int j) {
    std::vector<std::pair<int, int>> children;
    std::vector<std::pair<int, int>> moves = {{0, 1}, {1, 0}, {0, -1}, {-1, 0},
                                              {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    for (auto mov : moves) {
        if (map.is_walkable(mov.first + i, mov.second + j)) {
            if (mov.first * mov.second != 0) {
                if (!map.allow_diagonal)
                    continue;
                if (!map.allow_cut_corners) {
                    if (map.is_walkable(i, j + mov.second) && map.is_walkable(i + mov.first, j))
                        children.emplace_back(i + mov.first, j + mov.second);
                } else if (!map.allow_squeeze) {
                    if (map.is_walkable(i, j + mov.second) || map.is_walkable(i + mov.first, j))
                        children.emplace_back(i + mov.first, j + mov.second);
                } else children.emplace_back(i + mov.first, j + mov.second);
            } else children.emplace_back(i + mov.first, j + mov.second);
        }
    }
    return children;
}

double Search::get_hvalue(int i, int j) {
    int dx = std::abs(map.get_finish_x() - i);
    int dy = std::abs(map.get_finish_y() - j);
    switch (map.get_heuristic()) {
        case OCTILE: return (std::abs(dx - dy) + std::sqrt(2) * std::min(dx, dy));
        case MANHATTAN: return (dx + dy);
        case EUCLID: return std::sqrt(dx * dx + dy * dy);
        case CHEBYSHEV: return std::max(dx, dy);
    }
}

double Search::get_gsafety_min(int i, int j) {
    if (gsafety_min.count(std::make_pair(i, j)))
        return gsafety_min[std::make_pair(i, j)];
    return std::numeric_limits<double>::max();
}

void Search::print_solution(Node *node, std::ofstream &outfile) {
    outfile << std::fixed << std::setprecision(1);
    Node *current = node;
    auto **map_copy = new double *[map.get_height()];
    for (int i = 0; i < map.get_height(); ++i) {
        map_copy[i] = new double[map.get_width()];
        for (int j = 0; j < map.get_width(); ++j) {
            map_copy[i][j] = map.get_cell(i, j);
        }
    }
    while (current) {
        map_copy[current->i][current->j] = 2;
        current = current->parent;
    }
    for (int i = 0; i < map.get_height(); ++i) {
        outfile << "\t\t\t<row>";
        for (int j = 0; j < map.get_width(); ++j) {
            if (map_copy[i][j] == 2) {
                outfile << "*   ";
            } else {
                outfile << map_copy[i][j] << " ";
            }
        }
        outfile << "</row>\n";
    }
}
