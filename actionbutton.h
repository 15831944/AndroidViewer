#ifndef ACTIONBUTTON_H
#define ACTIONBUTTON_H

#include <QPushButton>

class ActionButton : public QPushButton{

    Q_OBJECT

public:
    ActionButton();

public slots:
    void setValue(int value);

signals:
    void valueChanged(int newValue);

private:
    int m_value;
};

#endif // ACTIONBUTTON_H
