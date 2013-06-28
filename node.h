#ifndef NODE_H
#define NODE_H

#include <QWidget>

enum m_type{
   ROOT, ROW,COL,BUTTON, TOGGLE, POPLIST, SLIDER, TABCTRL, TAB, OPENGL, RADIOGROUP, TEXT, EDIT
};

class Node {
public:
    m_type type;

    Node * parent;
    QWidget * myWidget;
    std::vector<Node*> children;

    //bool isRow;
    bool isLeaf;//if false, it is a container (layout or tab for now)

    const char * handle;
    const char * tooltip;
    const char * data;

    int length;
    int height;
    int caption;

    //empty node, probably used for rows or cols
    Node (){}

    //creates a leaf node with handle
    Node(const char * handle){
        this->handle=handle;
        this->isLeaf=true;
    }

    ~Node(){
        for (unsigned int i =0;i<children.size();i++)
            delete children.at(i);
    }

    void addChild(Node * n){
        this->children.push_back(n);
    }

    bool hasChildren(){
        return this->children.size()>0;
    }

    const char *  getType(){
        const char * str;
        switch (type){
        case ROW:
            str="Row";
            break;
        case COL:
            str="Col";
            break;
        case TOGGLE:
            str="Toggle";
            break;
        case BUTTON:
            str="Button";
            break;
        case POPLIST:
            str="PopList";
            break;
        case SLIDER:
            str="Slider";
            break;
        case OPENGL:
            str="OpenGL";
            break;
        case TABCTRL:
            str="Tab Control";
            break;
        case TAB:
            str="Tab";
            break;
        case TEXT:
            str="Text";
            break;
        case RADIOGROUP:
            str="RadioGroup";
            break;
        case ROOT:
            str="Root";
            break;
        case EDIT:
            str="Edit";
            break;
        default:
            str= "Unknown";
            break;

        }

        return str;
    }

};
#endif // NODE_H
