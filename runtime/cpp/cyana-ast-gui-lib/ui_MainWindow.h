/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *contentWidget;
    QVBoxLayout *verticalLayout;
    QWidget *graphicsViewWidget;
    QGridLayout *gridLayout_2;
    QGraphicsView *graphicsView;
    QWidget *scaleSliderWidget;
    QGridLayout *gridLayout;
    QSlider *scaleSlider;
    QWidget *exportImgBtnWidget;
    QGridLayout *gridLayout_4;
    QSpacerItem *leftHorizontalSpacer;
    QPushButton *exportImgBtn;
    QSpacerItem *rightHorizontalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(900, 800);
        contentWidget = new QWidget(MainWindow);
        contentWidget->setObjectName(QStringLiteral("contentWidget"));
        verticalLayout = new QVBoxLayout(contentWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        graphicsViewWidget = new QWidget(contentWidget);
        graphicsViewWidget->setObjectName(QStringLiteral("graphicsViewWidget"));
        gridLayout_2 = new QGridLayout(graphicsViewWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        graphicsView = new QGraphicsView(graphicsViewWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setMinimumSize(QSize(800, 700));

        gridLayout_2->addWidget(graphicsView, 0, 0, 1, 1);


        verticalLayout->addWidget(graphicsViewWidget);

        scaleSliderWidget = new QWidget(contentWidget);
        scaleSliderWidget->setObjectName(QStringLiteral("scaleSliderWidget"));
        scaleSliderWidget->setMinimumSize(QSize(0, 50));
        scaleSliderWidget->setMaximumSize(QSize(16777215, 50));
        gridLayout = new QGridLayout(scaleSliderWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        scaleSlider = new QSlider(scaleSliderWidget);
        scaleSlider->setObjectName(QStringLiteral("scaleSlider"));
        scaleSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(scaleSlider, 0, 0, 1, 1);


        verticalLayout->addWidget(scaleSliderWidget);

        exportImgBtnWidget = new QWidget(contentWidget);
        exportImgBtnWidget->setObjectName(QStringLiteral("exportImgBtnWidget"));
        exportImgBtnWidget->setMinimumSize(QSize(0, 50));
        exportImgBtnWidget->setMaximumSize(QSize(16777215, 50));
        gridLayout_4 = new QGridLayout(exportImgBtnWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        leftHorizontalSpacer = new QSpacerItem(386, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(leftHorizontalSpacer, 0, 0, 1, 1);

        exportImgBtn = new QPushButton(exportImgBtnWidget);
        exportImgBtn->setObjectName(QStringLiteral("exportImgBtn"));

        gridLayout_4->addWidget(exportImgBtn, 0, 1, 1, 1);

        rightHorizontalSpacer = new QSpacerItem(385, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(rightHorizontalSpacer, 0, 2, 1, 1);


        verticalLayout->addWidget(exportImgBtnWidget);

        MainWindow->setCentralWidget(contentWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        exportImgBtn->setText(QApplication::translate("MainWindow", "\345\257\274\345\207\272\344\270\272\345\233\276\347\211\207", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
