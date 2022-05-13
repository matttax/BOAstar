#include <iomanip>
#include "Search.h"

Search::Search(const std::string& file) {
    if(!map.read_map(file))
        exit(-1);
}

std::vector<Node *> Search::boa_star() {
    std::vector<Node*> solutions;
    long nodes_created = 1;
    long skips = 0;
    Node *start = new Node(map.get_start_y(), map.get_start_x(), 0, map.get_cell(map.get_start_y(), map.get_start_x()),
                           get_hvalue(map.get_start_y(), map.get_start_x()), nullptr);
    Open open;
    open.add(start);
    while (!open.empty()) {
        Node *current = open.top();
        //std::cout<<current->i<<" "<<current->j<<"\n";
        if (current->g_danger >= get_min_gdanger(current->i, current->j) ||
            current->f_danger >= get_min_gdanger(map.get_finish_y(), map.get_finish_x())) {
            skips++;
            continue;
        }
        gdanger_min[std::make_pair(current->i, current->j)] = current->g_danger;
        if (current->i == map.get_finish_y() && current->j == map.get_finish_x()) {
            solutions.push_back(current);
            continue;
        }
        auto children = get_children(current->i, current->j);
        for (auto child : children) {
            double length = (std::abs(current->i - child.first) + std::abs(current->j - child.second) == 2) ? std::sqrt(2) : 1.0,
                   danger = map.get_cell(child.first, child.second) + 0.1 * length;
            Node *child_node = new Node(child.first, child.second, length, danger,
                                        get_hvalue(child.first, child.second), current);
            if (child_node->g_danger >= get_min_gdanger(child.first, child.second) ||
                child_node->f_danger >= get_min_gdanger(map.get_finish_y(), map.get_finish_x())) {
                skips++;
                continue;
            }
            nodes_created++;
            opened.emplace(child);
            open.add(child_node);
        }
    }
    std::cout << opened.size() << " nodes opened.\n";
    return solutions;
}

std::vector<std::pair<int, int>> Search::get_children(int i, int j) {
    std::vector<std::pair<int, int>> children;
    std::vector<std::pair<int, int>> moves = {{0, 1}, {1, 0}, {0, -1}, {-1, 0},
                                              {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    for (auto mov : moves) {
        if (map.is_walkable(mov.first + i, mov.second + j)) {
            if (mov.first * mov.second != 0) {
                if (!map.diagonal_allowed())
                    continue;
                if (!map.corners_allowed()) {
                    if (map.is_walkable(i, j + mov.second) && map.is_walkable(i + mov.first, j))
                        children.emplace_back(i + mov.first, j + mov.second);
                } else if (!map.squeeze_allowed()) {
                    if (map.is_walkable(i, j + mov.second) || map.is_walkable(i + mov.first, j))
                        children.emplace_back(i + mov.first, j + mov.second);
                } else children.emplace_back(i + mov.first, j + mov.second);
            } else children.emplace_back(i + mov.first, j + mov.second);
        }
    }
    return children;
}

double Search::get_hvalue(int i, int j) {
    int dx = std::abs(map.get_finish_y() - i);
    int dy = std::abs(map.get_finish_x() - j);
    switch (map.get_heuristic()) {
        case OCTILE: return (std::abs(dx - dy) + std::sqrt(2) * std::min(dx, dy));
        case MANHATTAN: return (dx + dy);
        case EUCLID: return std::sqrt(dx * dx + dy * dy);
        case CHEBYSHEV: return std::max(dx, dy);
    }
}

double Search::get_min_gdanger(int i, int j) {
    if (gdanger_min.count(std::make_pair(i, j)))
        return gdanger_min[std::make_pair(i, j)];
    return std::numeric_limits<double>::max();
}

void Search::print_solution(Node *node, std::ofstream &outfile) {
    outfile << "\t\t<solution>\n";
    outfile << "\t\t\t<length>" << node->f_length << "</length>\n";
    outfile << "\t\t\t<danger>" << node->f_danger << "</danger>\n";
    outfile << "\t\t\t<path>\n";
    Node *current = node;
    std::vector<std::pair<int, int>> nodes;
    while (current) {
        nodes.emplace_back(current->i, current->j);
        current = current->parent;
    }
    for (int i = 0; i < map.get_height(); ++i) {
        outfile << "\t\t\t\t<row>";
        for (int j = 0; j < map.get_width(); ++j) {
            if (std::count(nodes.begin(), nodes.end(), std::make_pair(i, j))) {
                outfile << "*   ";
            } else {
                outfile << map.get_cell(i, j) << " ";
            }
        }
        outfile << "</row>\n";
    }
    outfile << "\t\t\t</path>\n";
    outfile << "\t\t\t<nodes>\n";
    for (auto nod = nodes.crbegin() ; nod != nodes.crend() ; nod++){
        outfile << "\t\t\t\t<node>" << nod->first << " " << nod->second << "</node>\n";
    }
    outfile << "\t\t\t</nodes>\n";
    outfile << "\t\t</solution>\n";
}

void Search::print_map(std::ofstream &outfile) {
    outfile << std::fixed << std::setprecision(1);
    outfile << "\t<map>\n";
    outfile << "\t\t<height>"<< map.get_height() << "</height>\n";
    outfile << "\t\t<width>"<< map.get_width() << "</width>\n";
    outfile << "\t\t<grid>\n";
    for (int i = 0; i < map.get_height(); ++i) {
        outfile << "\t\t\t<row>";
        for (int j = 0; j < map.get_width(); ++j)
            outfile << map.get_cell(i, j) << " ";
        outfile << "</row>\n";
    }
    outfile << "\t\t</grid>\n";
    outfile << "\t\t<startx>" << map.get_start_y() << "</startx>\n";
    outfile << "\t\t<starty>" << map.get_start_x() << "</starty>\n";
    outfile << "\t\t<finishx>" << map.get_finish_y() << "</finishx>\n";
    outfile << "\t\t<finishy>" << map.get_finish_x() << "</finishy>\n";
    outfile << "\t</map>\n";
}

void Search::print_opened(std::ofstream &outfile) {
    outfile << "\t<opened>\n";
    for (auto node: opened)
        outfile << "\t\t<node>" << node.first << " " << node.second << "</node>\n";
    outfile << "\t</opened>\n";
}

bool Search::visualize() {
    return map.do_visualize();
}

std::string Search::get_logfile() {
    return map.get_logfile_name();
}
