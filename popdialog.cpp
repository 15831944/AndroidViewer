#include "popdialog.h"
#include <iostream>

#include <QPushButton>
#include <QToolTip>
#include <QWidget>

#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QComboBox>
#include <QLabel>
#include <QRadioButton>
#include <QApplication>
#include <QtWidgets>

#include "qslider2.h"

#include "painter.h"


PopDialog::PopDialog(QWidget *parent) : QDialog(parent)
{
}


void PopDialog::display(){
    setLayout(this->buildLayout());
}

void PopDialog::SetWindowTitle(const char *text){
    this->setWindowTitle(text);
}

void PopDialog::SetCurrentTab(const char * tabControl, unsigned int tab){
    if (!handleMap[tabControl]){
        cout<<"That tab control doesn't exist"<<endl;
        return;
    }
    else if (handleMap[tabControl]->children.size()>=tab){
        Node * tabRoot = handleMap[tabControl];
        currentParent= tabRoot->children.at(tab);
    }
    else{
        cout<<"Tab doesn't exist"<<endl;
        return;
    }
}


void PopDialog::AddTabCtrl(const char * tabControl, int length, int height){
    Node * node= new Node(tabControl);
    node->isLeaf=false;
    node->type=TABCTRL;
    addNode(node);
}


void PopDialog::AddTab(const char * tabControl, char * name){
    if (!handleMap[tabControl]){
        cout<<"That tab control doesn't exist"<<endl;
        return;
    }
    else{
        Node * dad = handleMap[tabControl];
        Node * n = new Node();
        n->isLeaf=false;
        n->type=TAB;
        n->parent=dad;
        n->data=name;
        dad->addChild(n);
    }
}


void PopDialog::ActionTile(const char * handle/*,
                void (*func)(void *, char *, int )*/){
    //void * ptr=func;
    if (!handleMap[handle])
        std::cout<<"Handle not in layout yet"<<endl;
    else{
        Node * n = handleMap[handle];
        QPushButton * qpb = dynamic_cast<QPushButton*>(n->myWidget);
      //  connect (qpb,&QPushButton::clicked,func);
    }

}

void PopDialog::AddText(const char * handle,
                        const char * text){

    Node * n = new Node(handle);
    QLabel * qlb = new QLabel(text);
    n->myWidget=qlb;
    n->type=TEXT;
    addNode(n);
}

void PopDialog::AddEdit(const char *tile, int length, const char *value){
    Node * n = new Node(tile);
    QLineEdit * qle=new QLineEdit(value);
    qle->setMinimumWidth(length);
    qle->setMaximumWidth(length);
    n->myWidget=qle;
    n->type=EDIT;
    addNode(n);

}

void PopDialog::AddButton(const char* handle,
                          const char * caption,
                          int length){

    Node * n = new Node(handle);
    QPushButton * qbp = new QPushButton(caption);
    qbp->setMinimumWidth(length);
    n->myWidget=qbp;
    n->data=caption;
    n->type=BUTTON;
    addNode(n);
}

void PopDialog::AddToggle(const char * handle,
                          int length,
                          const char * caption){

    Node * n = new Node(handle);
    QRadioButton *qrb = new QRadioButton(caption);
    qrb->setMinimumWidth(length);
    n->myWidget=qrb;
    n->data=handle;
    n->type=TOGGLE;
    addNode(n);
}

//@TODO this should have its own class which includes labels for min and max
void PopDialog::AddSlider(const char * handle,
                          int length,
                          int height,
                          int style,
                          int min,
                          int max){

    bool vert = style==0 ? true: false;
    Node * n = new Node(handle);
    QSlider2 * qsp = new QSlider2(vert,min,max);

    qsp->setMinimumWidth(length);
    qsp->setMinimumHeight(height);
    qsp->setMaxText("asdasd");

    n->myWidget=qsp;
    n->data=handle;
    n->type=SLIDER;
    addNode(n);
}



void PopDialog::AddPopList(const char * handle,
                           int len){

    Node * n = new Node(handle);
    QComboBox * qcb = new QComboBox();
    qcb->setMinimumWidth(len);
    n->data=handle;
    n->type=POPLIST;
    n->myWidget=qcb;
    addNode(n);
}

void PopDialog::ListAdd(const char * handle,
                        const char * line){

    if (!handleMap[handle])
        std::cout<<"Handle not in layout yet"<<endl;
    else{
        Node * n= handleMap[handle];
        QComboBox * qcb = dynamic_cast<QComboBox*>(n->myWidget);
        QString qst(line);
        qcb->addItem(qst);
    }
}


void PopDialog::SetTileTip(const char * handle,
                           const char * value){

    if (!handleMap[handle])
        std::cout<<"Handle not in layout yet"<<endl;
    else{
        QWidget * widget = (handleMap[handle])->myWidget;
        widget->setToolTip(QString(value));
    }
}

void PopDialog::SetBitmap(const char * handle,
                          int id,
                          const char * fil){

    if (!handleMap[handle])
        std::cout<<"Handle not in layout yet"<<endl;
    else{
        Node * n = handleMap[handle];

        if (n->type==BUTTON){
            QPushButton * button = dynamic_cast<QPushButton*>(n->myWidget);
            QString theFile= QString(fil);
            QPixmap imDisplay(theFile);
            QIcon icon(imDisplay);
            button->setIcon(icon);
        }
    }



}




void PopDialog::AddGLView(const char * handle,
                          int width,
                          int height){

    Node * n = new Node(handle);
    Painter * glw = new Painter;
    glw->resize(500,500);
   // glw->update();
    //glw->setEnabled(true);
    n->type=OPENGL;
    n->myWidget=glw;
    addNode(n);
}


//#include "glwidget.h"


// #include <QGLWidget>
//#include <QtOpenGL/QGLWidget>
/*
void QPushButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
        emit rightClicked();
}
*/
