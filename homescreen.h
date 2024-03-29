#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>

namespace Ui {
class HomeScreen;
}

class HomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = nullptr);
    ~HomeScreen();

private slots:
    void on_pushButton_clicked();

private:
    Ui::HomeScreen *ui;


signals:
    void startGame();
};





#endif // HOMESCREEN_H
