#ifndef BOASTAR_NODE_H
#define BOASTAR_NODE_H

#include <tuple>

class Node {
public:
    int i, j;
    double g_length, g_danger;
    double f_length, f_danger;
    Node *parent;

    Node(int i, int j, double g_length, double g_safety, double h, Node* parent) {
        this->i = i;
        this->j = j;
        if (parent) {
            this->g_length = g_length + parent->g_length;
            this->g_danger = g_safety + parent->g_danger;
        } else {
            this->g_length = g_length;
            this->g_danger = g_safety;
        }
        f_length = this->g_length + h;
        f_danger = this->g_danger + h * 0.1;
        this->parent = parent;
    }
};

struct NodeComparator {
    bool operator()(const Node* n1, const Node* n2) const {
        if (n1->f_length == n2->f_length)
            return n1->f_danger > n2->f_danger;
        return n1->f_length > n2->f_length;
    }
};

#endif //BOASTAR_NODE_H
