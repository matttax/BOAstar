#ifndef BOASTAR_OPEN_H
#define BOASTAR_OPEN_H

#include <set>
#include "Node.h"

class Open {
private:
std::set<Node*, NodeComparator> open;

public:
    void add(Node* node);
    Node *top();
    bool empty();
    void print_open();
};


#endif //BOASTAR_OPEN_H
