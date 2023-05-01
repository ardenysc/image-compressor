#include "qtcount.h"

/*
A node is pruned if all of the leaves in its subtree have 
colour-distance within tolerance of its average. 
Distances between colours are computed as the sum, over each colour channel, 
of the pixel value differences, squared.

node is prunable if all of the leaves in the subtree rooted at that node 
have colour-distance to the node's average less than tolerance.
*/
bool qtcount::prunable(Node* node, const int tol) const {
    if (!node) {
        return false;
    }
    return isPrunable(node, node, tol);
}

bool qtcount::isPrunable(Node* croot, Node* node, const int tol) const {
    if (!node) {
        return false;
    }
    if (!node->NW && !node->NE && !node->SW && !node->SE) {
        double dis = pow(abs(croot->avg.r - node->avg.r), 2);
        dis += pow(abs(croot->avg.g - node->avg.g), 2);
        dis += pow(abs(croot->avg.b - node->avg.b), 2);
        return dis <= tol;
    } else {
        return isPrunable(croot, node->NW, tol) && isPrunable(croot, node->NE, tol) 
        && isPrunable(croot, node->SW, tol) && isPrunable(croot, node->SE, tol);
    }

}
