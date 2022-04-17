#ifndef BOASTAR_NODE_H
#define BOASTAR_NODE_H


class Node {
public:
    int i, j;
    double g_length, g_safety;
    double f_length, f_safety;
    Node *parent;

    Node(int i, int j, double g_length, double g_safety, double h, Node* parent) {
        this->i = i;
        this->j = j;
        if (parent) {
            this->g_length = g_length + parent->g_length;
            this->g_safety = g_safety + parent->g_safety;
        } else {
            this->g_length = g_length;
            this->g_safety = g_safety;
        }
        f_length = this->g_length + h;
        f_safety = this->g_safety + h;
        this->parent = parent;
    }
};

struct NodeComparator {
    bool operator()(const Node* n1, const Node* n2) {
        if (n1->f_safety < n2->f_safety && n1->f_length < n2->f_length)
            return false;
        return true;
    }
};

#endif //BOASTAR_NODE_H
