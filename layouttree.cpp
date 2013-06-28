#include "layouttree.h"
#include <vector>
//#include <QPushButton>
//#include <QRadioButton>
//#include <QComboBox>
#include <QTableWidget>
#include "node.h"
#include <iostream>
using namespace std;



void LayoutTree::StartColumn(){
    Node * n = new Node();
    n->isLeaf=false;
    n->type=COL;
    n->parent=currentParent;
    currentParent->addChild(n);
    lastParent=currentParent;
    currentParent=n;


}

void LayoutTree::EndColumn(){
    //currentParent=lastParent;
    currentParent=currentParent->parent;
}

void LayoutTree::StartRow(){

    Node * n = new Node();
    n->isLeaf=false;
    n->type=ROW;
    n->parent=currentParent;
    currentParent->addChild(n);
    lastParent=currentParent;
    currentParent=n;
}

void LayoutTree::EndRow(){
    currentParent=currentParent->parent;
}

/*
// -1 if there are nest issues (tabs within tabs, rows within rows
// 1 if its all good and the root is a col
// 2 if its all good and the root is a row
int validate(Node * root){
    int type=-1;
    vector<Node *> children = root->children;


    if (root->hasChildren()){
        for (int i=0;i<children.size;i++){


        }
    }

}
*/

int calls=0;


//The seed node has to be of type tabctrl and only have tab children
QTabWidget * LayoutTree::buildTabCtrl(Node * seed){
    QTabWidget * tabRoot=new QTabWidget;
    QWidget * tmpWidget;// = new QWidget;
    QBoxLayout * tmpLayout;

    vector<Node *> children= seed->children;
    vector<Node *>::iterator child=children.begin();

    for (; child != children.end(); child++){
        tmpWidget = new QWidget;
        tmpLayout= buildLayout(*child);
        tmpWidget->setLayout(tmpLayout);
        tabRoot->addTab(tmpWidget,(*child)->data);
    }

    return tabRoot;

}



/*returns a qboxlayout object with all of the
 * nodes contained under the tree that seed is the root of

 @TODO fix void pointers and cast everything correctly

*/
QBoxLayout * LayoutTree::buildLayout(Node * seed){
    seed = seed==NULL ? root:seed;


    int type=-1;
    QBoxLayout * box;
    vector<Node *> children= seed->children;
    vector<Node* >::iterator child=children.begin();


    for (;child !=children.end();child++){
        if ((*child)->type==ROW) type=2;
        if ((*child)->type==COL) type=1;
    }

    if (type==-1 || type==2)//either no chidlren or they are cols
        box=new QVBoxLayout;
    else if (type==1) //children are rows
        box=new QHBoxLayout;


    for (child=children.begin();child!=children.end();child++){
        if ((*child)->type==TABCTRL){
            box->addWidget(buildTabCtrl(*child));
        }
        else if ((*child)->isLeaf){
            QWidget * widget = (*child)->myWidget;
            box->addWidget(widget);
        }
        else{ //children should be layouts
            box->addLayout(buildLayout(*child));
        }
    }

   /* for (unsigned int i=0; i< children.size();i++){
        if (children.at(i)->isLeaf){
            Node * child = children.at(i);
            QWidget * widget=child->myWidget;
            box->addWidget(widget);
        }
        else{
            box->addLayout(static_cast<QBoxLayout *> (buildLayout(children.at(i))));
        }
    }*/

    return box;



}

    /*
    if (seed->type==TABCTRL){

    }
    else if (seed->type==TAB){

    }
    else if (seed->type==ROOT){
        int type=-1;
        QBoxLayout * box;
        vector<Node *> children= seed->children;
        vector<Node* >::iterator child=children.begin();

        for (;child !=children.end();child++){
            if ((*child)->type==ROW) type=1;
            if ((*child)->type==COL) type=2;
        }
        if (type==-1 || type==2)//either no chidlren or they are cols
            box=new QVBoxLayout;
        else if (type==1)
            box=new QHBoxLayout;


        for (unsigned int i=0; i< children.size();i++){
            if (children.at(i)->isLeaf){
                Node * child = children.at(i);
                QWidget * widget=child->myWidget;
                box->addWidget(widget);
            }
            else{
                box->addLayout(static_cast<QBoxLayout *> (buildLayout(children.at(i))));
            }
        }

        return box;

    }
    else{
        QBoxLayout * box;

        if (seed->type==ROW)
            box = new QHBoxLayout();
        else
            box = new QVBoxLayout();

        vector <Node *> children= seed->children;
        for (unsigned int i=0; i< children.size();i++){
            if (children.at(i)->isLeaf){
                Node * child = children.at(i);
                QWidget * widget=child->myWidget;
                box->addWidget(widget);
            }
            else{
                box->addLayout(static_cast<QBoxLayout *> (buildLayout(children.at(i))));
            }
        }

        return box;
    }*/


   // cout <<"Build layout call #"<<++calls<<endl;
/*
    seed = (seed==NULL) ? root : seed;

    QBoxLayout * box;
    QTabWidget * tabCtrl;

    vector<Node *> children= seed->children;
    vector<Node* >::iterator child=children.begin();

    switch (seed->type){
    case ROOT:{
        QBoxLayout * theBox;
        int type=-1;

        for (;child !=children.end();child++){
            if ((*child)->type==ROW) type=1;
            if ((*child)->type==COL) type=2;
        }

        if (type==-1 || type==2)//either no chidlren or they are cols
            theBox=new QVBoxLayout;
        else if (type==1)
            theBox=new QHBoxLayout;


        for (child = children.begin();child!=children.end();child++){
            cout<< "Root has "<<++calls<<endl;
            if ((*child)->isLeaf){
                theBox->addWidget((*child)->myWidget);
            }else{

                    theBox->addLayout(static_cast<QHBoxLayout *> (buildLayout(*child)));
            }
        }
        return theBox;}
    case ROW:
        box = new QHBoxLayout();

        for (;child!=children.end();child++){
            if ((*child)->type==ROW){
                cout<<"Can't nest Rows and Rows"<<endl;
                return NULL;
            }

            if ((*child)->isLeaf){
                box->addWidget((*child)->myWidget);
            }else if ((*child)->type == TABCTRL){
                box->addWidget(static_cast<QTabWidget *>(buildLayout(*child)));

            }else{
                box->addLayout(static_cast<QVBoxLayout *> (buildLayout(*child)));
            }
        }
        return box;
    case COL:
        box = new QVBoxLayout();
        for (;child!=children.end();child++){
            if ((*child)->type==COL){
                cout<<"Can't nest Cols and Cols"<<endl;
                return NULL;
            }

            if ((*child)->isLeaf){
                box->addWidget((*child)->myWidget);
            }else if ((*child)->type == TABCTRL){
                box->addWidget(static_cast<QTabWidget *>(buildLayout(*child)));

            }else{
                box->addLayout(static_cast<QHBoxLayout *> (buildLayout(*child)));
            }
        }
       return box;
    case TABCTRL:
         tabCtrl = new QTabWidget;
         QWidget * tmp;

         for (;child!=children.end();child++){
             tmp=static_cast<QWidget *> (buildLayout(*child));
             tabCtrl->addTab(tmp,(*child)->data);
         }
        return tabCtrl;
       // break;

    case TAB:
    {
        QWidget * theWidget;
        QBoxLayout * theBox;
        int type=-1;

        for (;child !=children.end();child++){
            if ((*child)->type==ROW) type=1;
            if ((*child)->type==COL) type=2;
        }


        if (type==-1 || type==2)//either no chidlren or they are cols
            theBox=new QVBoxLayout;
        else if (type==1)
            theBox=new QHBoxLayout;


        for (child = children.begin();child!=children.end();child++){
            if ((*child)->isLeaf){
                theBox->addWidget((*child)->myWidget);
            }else{
                theBox->addLayout(static_cast<QBoxLayout *> (buildLayout(*child)));
            }
        }
        theWidget->setLayout(theBox);

        return theWidget;
    }
    default: // this is assuming the object is a widget
        QBoxLayout * theBox;
        return theBox;

*/




   /* QBoxLayout * box;

    if (seed->type==ROW)
        box = new QHBoxLayout();
    else
        box = new QVBoxLayout();

    vector <Node *> children= seed->children;
    for (unsigned int i=0; i< children.size();i++){
        if (children.at(i)->isLeaf){
            Node * child = children.at(i);
            QWidget * widget=child->myWidget;
            box->addWidget(widget);
        }
        else{
            box->addLayout(buildLayout(children.at(i)));
        }
    }

    return box;*/


