#ifndef QSLIDER2_H
#define QSLIDER2_H

#include <QWidget>

class QSlider2 : public QWidget
{
    Q_OBJECT
public:
   //explicit QSlider2(QWidget *parent = 0);
    explicit QSlider2(bool vert= false, int min =0, int max=0);

    void setMaximumHeight(int minh);

    void setMaximumWidth(int minw);

    void setMaxText(const char * text);

    void setMinText(const char * text);

signals:
    
public slots:
    

private:
    QWidget * minThing;
    QWidget * maxThing;
    QWidget * theSlider;
};

#endif // QSLIDER2_H
