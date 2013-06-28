#include "tree.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

void printTabs(int n){
    cout<< string(n,'  ');
}

Tree::Tree(){
    root=currentParent=lastParent=new Node();
    root->isLeaf=false;
    root->type=ROOT;
    currentLevel=0;
}

Tree::~Tree(){
    delete root;
}

//adds a node at the current tree position
void Tree::addNode(Node * node){
    handleMap[node->handle]= node;
    node->parent=currentParent;
    currentParent->addChild(node);
}

void Tree::printTree(Node * seed){
    if (seed==NULL)
        seed=root;

    printTabs(currentLevel);
    if (seed->isLeaf){
        printTabs(currentLevel-2);
        cout<<seed->getType()<<": "<<seed->handle<<endl;
        return;
    }
    else{
        cout<< "^" << seed->getType() <<": " <<seed<<endl;
        currentLevel++;
        for (unsigned int i = 0; i < seed->children.size(); i++){
            printTree(seed->children.at(i));
        }
        currentLevel--;

        printTabs(currentLevel);
        cout<< "~" << seed->getType() <<": " <<seed<<endl;
    }
}


