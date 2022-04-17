#ifndef BOASTAR_OPEN_H
#define BOASTAR_OPEN_H

#include <vector>
#include "Node.h"

class Open {
private:
std::vector<Node*> open;

public:
    void add(Node* node);
    Node *top();
    bool empty();
};


#endif //BOASTAR_OPEN_H
