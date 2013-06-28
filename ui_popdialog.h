/********************************************************************************
** Form generated from reading UI file 'popdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POPDIALOG_H
#define UI_POPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_PopDialog
{
public:

    void setupUi(QDialog *PopDialog)
    {
        if (PopDialog->objectName().isEmpty())
            PopDialog->setObjectName(QStringLiteral("PopDialog"));
        PopDialog->resize(400, 300);

        retranslateUi(PopDialog);

        QMetaObject::connectSlotsByName(PopDialog);
    } // setupUi

    void retranslateUi(QDialog *PopDialog)
    {
        PopDialog->setWindowTitle(QApplication::translate("PopDialog", "PopDialog", 0));
    } // retranslateUi

};

namespace Ui {
    class PopDialog: public Ui_PopDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POPDIALOG_H
