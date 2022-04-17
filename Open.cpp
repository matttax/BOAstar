#include "Open.h"

void Open::add(Node *node) {
    open.push_back(node);
}

Node *Open::top() {
    Node *max = open.at(0);
    int max_index = 0;
    for (int i = 1; i < open.size(); ++i) {
        if (max->f_safety <= open[i]->f_safety && max->f_length < open[i]->f_length) {
            max = open[i];
            max_index = i;
        }
    }
    open.erase(open.begin() + max_index);
    return max;
}

bool Open::empty() {
    return open.empty();
}
