
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

quadtree::quadtree(PNG& imIn) {
    int dim = min(log2(imIn.width()), log2(imIn.height()));
    edge = pow(2, dim);
    imIn.resize(edge, edge);
    stats s = stats(imIn);
    root = buildTree(s, {0, 0}, dim);
}

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    if (dim == -1) {
        return nullptr;
    }

    int half_len = (int) pow(2, dim) / 2;

    pair<int, int> nw = {ul.first, ul.second};
    pair<int, int> ne = {ul.first + half_len, ul.second};
    pair<int, int> sw = {ul.first, ul.second + half_len};
    pair<int, int> se = {ul.first + half_len, ul.second + half_len};

    RGBAPixel a = s.getAvg(ul, dim);
    double v = s.getVar(ul, dim);
    Node *croot = new Node(ul, dim, a, v);

    croot->NW = buildTree(s, nw, dim - 1);
    croot->NE = buildTree(s, ne, dim - 1);
    croot->SW = buildTree(s, sw, dim - 1);
    croot->SE = buildTree(s, se, dim - 1);

    return croot;
}

PNG quadtree::render() const {
    PNG img = PNG(edge, edge);

    renderTree(img, root);
    
    return img;
}

void quadtree::renderTree(PNG &img, Node *croot) const {
    if (!croot) {
        return;
    }

    if (!croot->NW && !croot->NE && !croot->SW && !croot->SE) {
        for (unsigned int x = croot->upLeft.first; x < croot->upLeft.first + pow(2, croot->dim); x++) {
            for (unsigned int y = croot->upLeft.second; y < croot->upLeft.second + pow(2, croot->dim); y++) {
                *img.getPixel(x, y) = croot->avg;
            }
        }
        return;
    }
    
    renderTree(img, croot->NW);
    renderTree(img, croot->NE);
    renderTree(img, croot->SW);
    renderTree(img, croot->SE);
}

/*
 * The idealPrune function can be considered to be the inverse
 * of the pruneSize function. It takes as input a number of leaves
 * and returns the minimum tolerance that would produce that resolution
 * upon a prune. It does not change the structure of the tree.
 * returns the smallest tolerance such that pruneSize(tol) <= leaves
 */
// int quadtree::idealPrune(const int leaves) const {
//     /* Your code here! */
//     int bound = 1;
//     while (pruneSize(bound) >= leaves) {
//         bound = bound * 2;
//     }

//     int high_tol = bound; // less leaves
//     int low_tol = 0; // more leaves
//     while (low_tol <= high_tol) {
//         int mid_tol = ((high_tol - low_tol) / 2) + low_tol;
//         if (pruneSize(mid_tol) == leaves) {
//             return mid_tol;
//         }
//         else if (pruneSize(mid_tol) > leaves) {
//             low_tol = mid_tol + 1;
//         } else {
//             high_tol = mid_tol - 1;
//         }
//         printf("(%d, %d, %d) ", low_tol, mid_tol, high_tol);
//         printf("(%d, %d, %d)\n", pruneSize(low_tol), pruneSize(mid_tol), pruneSize(high_tol));
//     }
//     printf("end\n");
//     return low_tol;
// }
int quadtree::idealPrune(const int leaves) const {
    if (pow(edge, 2) <= leaves) {
        return 0;
    }
    
    int low_tol = 0;
    int unit = pow(2, 10);
    while (unit) {
        if (pruneSize(low_tol + unit) > leaves) {
            low_tol += unit;
        } else {
            unit = unit / 2;
        }
    }
    return low_tol + 1;
}


/*
 * The pruneSize function takes a tolerance as input, and returns
 * the number of leaves that would result if the tree were to
 * be pruned with that tolerance. Consistent with the definition
 * of prune, a node is counted if prunable returns true.
 */

int quadtree::pruneSize(const int tol) const {
    return countPrune(root, tol);
}

int quadtree::countPrune(Node *croot, const int tol) const {
    if (!croot) {
        return 0;
    }
    if (prunable(croot, tol)) {
        return 1;
    } else {
        return countPrune(croot->NW, tol) + countPrune(croot->NE, tol) + countPrune(croot->SW, tol) + countPrune(croot->SE, tol);
    }
}

void quadtree::prune(const int tol) {
    pruneTree(root, tol);
}

void quadtree::pruneTree(Node *croot, const int tol) {
    if (!croot) {
        return;
    }

    if (prunable(croot, tol)) {
        clearTree(croot->NW);
        croot->NW = nullptr;
        clearTree(croot->NE);
        croot->NE = nullptr;
        clearTree(croot->SW);
        croot->SW = nullptr;
        clearTree(croot->SE);
        croot->SE = nullptr;
    } else {
        pruneTree(croot->NW, tol);
        pruneTree(croot->NE, tol);
        pruneTree(croot->SW, tol);
        pruneTree(croot->SE, tol);
    }
}

void quadtree::clear() {
    clearTree(root);
}

void quadtree::clearTree(Node *croot) {
    if (!croot) {
        return;
    }

    clearTree(croot->NW);
    clearTree(croot->NE);
    clearTree(croot->SW);
    clearTree(croot->SE);

    delete croot;
    croot = NULL;
}

void quadtree::copy(const quadtree& orig) {
    /* your code here */
    if (!orig.root) {
        return;
    }

    root = copyTree(orig.root);
    edge = orig.edge;
}

quadtree::Node* quadtree::copyTree(Node *ocroot) {
    if (!ocroot) {
        return nullptr;
    }
    
    Node *croot = new Node(ocroot->upLeft, ocroot->dim, ocroot->avg, ocroot->var);

    croot->NW = copyTree(ocroot->NW);
    croot->NE = copyTree(ocroot->NE);
    croot->SW = copyTree(ocroot->SW);
    croot->SE = copyTree(ocroot->SE);

    return croot;
}
