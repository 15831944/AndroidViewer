#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>

class Painter : public QWidget
{
    Q_OBJECT
public:
    explicit Painter(QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *);
signals:
    
public slots:
    
};

#endif // PAINTER_H
