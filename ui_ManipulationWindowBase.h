/********************************************************************************
** Form generated from reading UI file 'ManipulationWindowBase.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANIPULATIONWINDOWBASE_H
#define UI_MANIPULATIONWINDOWBASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ManipulationWindowBase
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QWidget *widgetView;
    QSlider *verticalSlider;
    QSpinBox *spinBox;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ManipulationWindowBase)
    {
        if (ManipulationWindowBase->objectName().isEmpty())
            ManipulationWindowBase->setObjectName(QString::fromUtf8("ManipulationWindowBase"));
        ManipulationWindowBase->resize(800, 600);
        centralwidget = new QWidget(ManipulationWindowBase);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widgetView = new QWidget(widget);
        widgetView->setObjectName(QString::fromUtf8("widgetView"));
        spinBox->raise();

        horizontalLayout->addWidget(widgetView);

        verticalSlider = new QSlider(widget);
        verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
        verticalSlider->setMinimum(-300);
        verticalSlider->setMaximum(0);
        verticalSlider->setValue(-100);
        verticalSlider->setOrientation(Qt::Vertical);
        verticalSlider->setTickPosition(QSlider::TicksBothSides);
        verticalSlider->setTickInterval(10);

        horizontalLayout->addWidget(verticalSlider);


        verticalLayout->addWidget(widget);

        spinBox = new QSpinBox(centralwidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        verticalLayout->addWidget(spinBox);

        ManipulationWindowBase->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ManipulationWindowBase);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        ManipulationWindowBase->setMenuBar(menubar);
        statusbar = new QStatusBar(ManipulationWindowBase);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ManipulationWindowBase->setStatusBar(statusbar);

        retranslateUi(ManipulationWindowBase);

        QMetaObject::connectSlotsByName(ManipulationWindowBase);
    } // setupUi

    void retranslateUi(QMainWindow *ManipulationWindowBase)
    {
        ManipulationWindowBase->setWindowTitle(QCoreApplication::translate("ManipulationWindowBase", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ManipulationWindowBase: public Ui_ManipulationWindowBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANIPULATIONWINDOWBASE_H
