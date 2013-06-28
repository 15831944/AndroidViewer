
#ifndef TREE_H
#define TREE_H

#include <vector>
#include "node.h"
#include <map>

using namespace std;

struct cmp{
    bool operator()(const char * c1, const char * c2) const {
        return strcmp(c1,c2)<0;
    }
};



//This is an implementaion of a tree with three node types, rows columns and leafs.
//Rows can only nest columns and columns can only nest rows. Both can nest leafs.
class Tree{
protected:

    Node * root;
    int currentLevel;
    Node * currentParent;
    Node * lastParent;
    map<const char *, Node *,cmp> handleMap;
public:
    Tree();
    ~Tree();

    void addNode(Node * leaf);
    void printTree(Node * seed=NULL);
};

#endif // TREE_H


