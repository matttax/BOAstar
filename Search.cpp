#include "Search.h"

std::string integer_to_Roman(int n) {
    std::string str_romans[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    std::string result;
    for (int i = 0; i < 13; ++i) {
        while(n - values[i] >= 0) {
            result += str_romans[i];
            n -= values[i];
        }
    }
    return result;
}

Search::Search() {
    std::cin >> height >> width;
    map = new double *[height];
    for (int i = 0; i < height; ++i) {
        map[i] = new double[width];
        for (int j = 0; j < width; ++j)
            std::cin >> map[i][j];
    }
    std::cin >> start_x >> start_y;
    std::cin >> finish_x >> finish_y;
    map[start_x][start_y] = 0;
}

std::vector<Node *> Search::boa_star() {
    std::vector<Node*> solutions;
    Node *start = new Node(start_x, start_y, 0, 0,
                           get_hvalue(start_x, start_y), nullptr);
    Open open;
    open.add(start);
    while (!open.empty()) {
        Node *current = open.top();
        if (current->g_safety >= get_gsafety_min(current->i, current->j) &&
            current->f_safety >= get_gsafety_min(finish_x, finish_y)) {
            continue;
        }
        gsafety_min[current->i * width + current->j] = current->g_safety;
        if (current->i == finish_x && current->j == finish_y) {
            current->f_safety -= current->f_length;
            solutions.push_back(current);
        }
        auto children = get_children(current->i, current->j);
        for (auto child : children) {
            double length = (std::abs(child.first) + std::abs(child.second) == 2) ? std::sqrt(2) : 1.0,
                   safety = map[child.first][child.second] + length;
            Node *child_node = new Node(child.first, child.second, length, safety,
                                       get_hvalue(child.first, child.second), current);
            if (child_node->g_safety >= get_gsafety_min(child.first, child.second) ||
                child_node->f_safety >= get_gsafety_min(finish_x, finish_y)) {
                continue;
            }
            open.add(child_node);
        }
    }
    return solutions;
}

std::vector<std::pair<int, int>> Search::get_children(int i, int j) const {
    std::vector<std::pair<int, int>> children;
    std::vector<std::pair<int, int>> moves = {{0, 1}, {1, 0}, {0, -1}, {-1, 0},
                                              {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    for (auto mov : moves) {
        if (mov.first + i >= 0 && mov.first + i < height && mov.second + j >= 0 && mov.second + j < width)
            if (map[mov.first + i][mov.second + j] != 1)
                children.emplace_back(mov.first + i, mov.second + j);
    }
    return children;
}

double Search::get_hvalue(int i, int j) const {
    int dx = std::abs(finish_x - i);
    int dy = std::abs(finish_y - j);
    return std::sqrt(dx * dx + dy * dy);
}

double Search::get_gsafety_min(int i, int j) {
    int hash = i * width + j;
    if (gsafety_min.count(hash))
        return gsafety_min[hash];
    return INT_MAX;
}

void Search::print_solution(Node *node) const {
    Node *current = node;
    auto **map_copy = new double *[height];
    for (int i = 0; i < height; ++i) {
        map_copy[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            map_copy[i][j] = map[i][j];
        }
    }
    std::map<std::pair<int, int>, int> nodes_queue;
    int order = 1;
    while (current) {
        nodes_queue[std::make_pair(current->i, current->j)] = order++;
        map_copy[current->i][current->j] = 2;
        current = current->parent;
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (map_copy[i][j] == 2) {
                std::cout << integer_to_Roman(nodes_queue[std::make_pair(i, j)]) << "\t";
                map_copy[i][j] = 0;
            } else {
                std::cout << map_copy[i][j] << "\t";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
