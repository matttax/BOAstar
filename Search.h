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
#include <unordered_set>

#include "Node.h"
#include "Open.h"
#include "Map.h"

struct pair_hash {
    inline std::size_t operator()(const std::pair<int,int> & v) const {
        return v.first*31+v.second;
    }
};

class Search {
private:
    Map map;
    std::unordered_set<std::pair<int, int>, pair_hash> opened;
    std::map<std::pair<int, int>, double> gdanger_min;

public:
    Search(const std::string& file);
    std::vector<Node*> boa_star();
    std::vector<std::pair<int, int>> get_children(int i, int j);
    double get_hvalue(int i, int j);
    double get_min_gdanger(int i, int j);
    void print_solution(Node* node, std::ofstream &outfile);
    void print_map(std::ofstream &outfile);
    void print_opened(std::ofstream &outfile);
};

#endif //BOASTAR_SEARCH_H
