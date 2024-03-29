/********************************************************************************
** Form generated from reading UI file 'homescreen.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOMESCREEN_H
#define UI_HOMESCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HomeScreen
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacerTop;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacerBottom;

    void setupUi(QWidget *HomeScreen)
    {
        if (HomeScreen->objectName().isEmpty())
            HomeScreen->setObjectName("HomeScreen");
        HomeScreen->resize(600, 400);
        verticalLayout = new QVBoxLayout(HomeScreen);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacerTop = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacerTop);

        pushButton = new QPushButton(HomeScreen);
        pushButton->setObjectName("pushButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        pushButton->setFont(font);

        verticalLayout->addWidget(pushButton, 0, Qt::AlignCenter);

        verticalSpacerBottom = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacerBottom);

        verticalLayout->setStretch(1, 1);

        retranslateUi(HomeScreen);

        QMetaObject::connectSlotsByName(HomeScreen);
    } // setupUi

    void retranslateUi(QWidget *HomeScreen)
    {
        HomeScreen->setWindowTitle(QCoreApplication::translate("HomeScreen", "Puissance 4 - Commencez \303\240 Jouer !", nullptr));
        HomeScreen->setStyleSheet(QCoreApplication::translate("HomeScreen", "\n"
"    QWidget {\n"
"        background-color: #34495e;\n"
"    }\n"
"   ", nullptr));
        pushButton->setText(QCoreApplication::translate("HomeScreen", "Commencer une partie !", nullptr));
        pushButton->setStyleSheet(QCoreApplication::translate("HomeScreen", "\n"
"        QPushButton {\n"
"            border: 1px solid #555;\n"
"            border-radius: 15px;\n"
"            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                              stop:0 #f6d365, stop:1 #fda085);\n"
"            padding: 10px 20px;\n"
"            color: #2c3e50;\n"
"        }\n"
"        QPushButton:hover {\n"
"            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                              stop:0 #fda085, stop:1 #f6d365);\n"
"        }\n"
"        QPushButton:pressed {\n"
"            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                                              stop:0 #e0a64b, stop:1 #f48942);\n"
"            padding-left: 15px;\n"
"            padding-top: 5px;\n"
"        }\n"
"       ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HomeScreen: public Ui_HomeScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMESCREEN_H
