#include "mainwindow.h"
#include "homescreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HomeScreen homeScreen;

    QObject::connect(&homeScreen, &HomeScreen::startGame, [&](){
        homeScreen.close();
        MainWindow* w = new MainWindow;
        w->show();
    });

    homeScreen.show();
    return a.exec();
}
