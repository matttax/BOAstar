#ifndef BOASTAR_SEARCH_H
#define BOASTAR_SEARCH_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
#include <cmath>
#include "Node.h"
#include "Open.h"
#include "Map.h"

class Search {
private:
    Map map;
    std::map<std::pair<int, int>, double> gsafety_min;

public:
    Search(const std::string& file);
    std::vector<Node*> boa_star();
    std::vector<std::pair<int, int>> get_children(int i, int j);
    double get_hvalue(int i, int j);
    double get_gsafety_min(int i, int j);
    void print_solution(Node* node, std::ofstream &outfile);
};

#endif //BOASTAR_SEARCH_H
