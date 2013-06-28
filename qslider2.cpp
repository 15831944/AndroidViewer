#include "qslider2.h"
#include <QSlider>
#include <QBoxLayout>
#include <QLabel>


void QSlider2::setMaximumHeight(int minh){
    theSlider->setMaximumHeight(minh);
}

void QSlider2::setMaximumWidth(int minw){
    theSlider->setMaximumWidth(minw);
    theSlider->resize(20,20);
}

void QSlider2::setMaxText(const char *text){
    QLabel * label = new QLabel(text);
    this->maxThing = label;
    maxThing->update();
    this->update();
}

void QSlider2::setMinText(const char * text){
    QLabel * label = new QLabel(text);
    this->minThing=label;

}

//true means horizontal
QSlider2::QSlider2(bool vert, int min, int max):
    QWidget(0)
{
    QBoxLayout * theBox;

    char * buf = new char;
    char * nothing = "";
    sprintf(buf,"%d",min);
    buf = min==0 ? nothing : buf;
    maxThing=new QLabel(buf);

    sprintf(buf,"%d",max);
    buf = max==0 ? nothing : buf;
    minThing=new QLabel(buf);


    if (vert){
        theSlider = new QSlider(Qt::Horizontal);
        theBox = new QHBoxLayout;
    }
    else{
        theSlider = new QSlider(Qt::Vertical);
        theBox= new QVBoxLayout;
    }

    theBox->addWidget(minThing);
    theBox->addWidget(theSlider);
    theBox->addWidget(maxThing);

    this->setLayout(theBox);
}
