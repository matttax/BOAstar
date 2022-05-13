#ifndef BOASTAR_OPEN_H
#define BOASTAR_OPEN_H

#include <set>
#include <queue>
#include "Node.h"

class Open {
private:
std::priority_queue<Node*, std::vector<Node*>, NodeComparator> open;

public:
    void add(Node* node);
    Node *top();
    bool empty();
};


#endif //BOASTAR_OPEN_H
