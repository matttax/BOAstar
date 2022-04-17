#ifndef BOASTAR_SEARCH_H
#define BOASTAR_SEARCH_H

#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include "Node.h"
#include "Open.h"

class Search {
public:
    int width{}, height{};
    double **map;
    int start_x{}, start_y{};
    int finish_x{}, finish_y{};
    std::map<int, double> gsafety_min;

    Search();
    std::vector<Node*> boa_star();
    std::vector<std::pair<int, int>> get_children(int i, int j) const;
    double get_hvalue(int i, int j) const;
    double get_gsafety_min(int i, int j);
    void print_solution(Node* node) const;
};

#endif //BOASTAR_SEARCH_H
