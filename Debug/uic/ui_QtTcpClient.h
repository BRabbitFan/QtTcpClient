/********************************************************************************
** Form generated from reading UI file 'QtTcpClient.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTTCPCLIENT_H
#define UI_QTTCPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_QtTcpClientClass
{
public:

    void setupUi(QDialog *QtTcpClientClass)
    {
        if (QtTcpClientClass->objectName().isEmpty())
            QtTcpClientClass->setObjectName(QString::fromUtf8("QtTcpClientClass"));
        QtTcpClientClass->resize(600, 400);

        retranslateUi(QtTcpClientClass);

        QMetaObject::connectSlotsByName(QtTcpClientClass);
    } // setupUi

    void retranslateUi(QDialog *QtTcpClientClass)
    {
        QtTcpClientClass->setWindowTitle(QCoreApplication::translate("QtTcpClientClass", "QtTcpClient", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtTcpClientClass: public Ui_QtTcpClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTTCPCLIENT_H
