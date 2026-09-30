/********************************************************************************
** Form generated from reading UI file 'SelectionWindowBase.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTIONWINDOWBASE_H
#define UI_SELECTIONWINDOWBASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectionWindowBase
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_3;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout_4;
    QListView *listView_Directories;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_Files;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SelectionWindowBase)
    {
        if (SelectionWindowBase->objectName().isEmpty())
            SelectionWindowBase->setObjectName(QStringLiteral("SelectionWindowBase"));
        SelectionWindowBase->resize(900, 600);
        SelectionWindowBase->setStyleSheet(QLatin1String("QGroupBox {\n"
"    border: 1px solid gray;\n"
"    border-radius: 9px;\n"
"    margin-top: 0.5em;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}\n"
""));
        centralwidget = new QWidget(SelectionWindowBase);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(groupBox);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        scrollArea_2 = new QScrollArea(groupBox);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollArea_2->sizePolicy().hasHeightForWidth());
        scrollArea_2->setSizePolicy(sizePolicy1);
        scrollArea_2->setFrameShape(QFrame::NoFrame);
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 274, 507));
        horizontalLayout_4 = new QHBoxLayout(scrollAreaWidgetContents_2);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        listView_Directories = new QListView(scrollAreaWidgetContents_2);
        listView_Directories->setObjectName(QStringLiteral("listView_Directories"));
        sizePolicy1.setHeightForWidth(listView_Directories->sizePolicy().hasHeightForWidth());
        listView_Directories->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(listView_Directories);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        horizontalLayout_3->addWidget(scrollArea_2);


        horizontalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        scrollArea = new QScrollArea(groupBox_2);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 562, 492));
        horizontalLayout_2 = new QHBoxLayout(scrollAreaWidgetContents);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        widget_Files = new QWidget(scrollAreaWidgetContents);
        widget_Files->setObjectName(QStringLiteral("widget_Files"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_Files->sizePolicy().hasHeightForWidth());
        widget_Files->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(widget_Files);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        horizontalLayout->addWidget(groupBox_2);

        SelectionWindowBase->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SelectionWindowBase);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 900, 25));
        SelectionWindowBase->setMenuBar(menubar);
        statusbar = new QStatusBar(SelectionWindowBase);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        SelectionWindowBase->setStatusBar(statusbar);

        retranslateUi(SelectionWindowBase);

        QMetaObject::connectSlotsByName(SelectionWindowBase);
    } // setupUi

    void retranslateUi(QMainWindow *SelectionWindowBase)
    {
        SelectionWindowBase->setWindowTitle(QApplication::translate("SelectionWindowBase", "Selection", 0));
        groupBox->setTitle(QApplication::translate("SelectionWindowBase", "Directories", 0));
        groupBox_2->setTitle(QApplication::translate("SelectionWindowBase", "Files", 0));
    } // retranslateUi

};

namespace Ui {
    class SelectionWindowBase: public Ui_SelectionWindowBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTIONWINDOWBASE_H
