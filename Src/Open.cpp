#include "Open.h"

void Open::add(Node *node) {
    open.push(node);
}

Node *Open::top() {
    Node* max = open.top();
    open.pop();
    return max;
}

bool Open::empty() {
    return open.empty();
}

