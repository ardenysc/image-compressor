#include "qtvar.h"

bool qtvar::prunable(Node* node, const int tol) const {
    // cout<<node->var<<endl;
    return node->var < tol;
}
