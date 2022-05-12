#include <iostream>
#include "Open.h"

void Open::add(Node *node) {
    open.insert(node);
}

Node *Open::top() {
    Node *max = *open.rbegin();
    auto i = --open.end();
    open.erase(i);
    return max;
}

bool Open::empty() {
    return open.empty();
}

void Open::print_open() {
    for (auto n : open) {
        std::cout << "[" << n->i << " " << n->j << " " << n->f_length << " " << n->f_danger << "]";
    }
    std::cout << "\n";
}
