#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum Player { Player_None, Player_Human, Player_AI, Player1, Player2, NoPlayer };
enum GameMode { VersusPlayer, VersusAI };

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleButtonClicked();

private:
    static const int FOUR_IN_A_ROW = 100000;
    static const int THREE_IN_A_ROW = 100;
    static const int TWO_IN_A_ROW = 10;
    Ui::MainWindow *ui;
    Player currentPlayer;
    Player gameBoard[6][7];
    bool isGameOver;
    GameMode gameMode;
    void initializeGame();
    void updateButtonUI(int row, int col);
    int findEmptyRow(int col);
    bool checkVictory(int row, int col);
    void changePlayer();
    void updateUI();
    void gameOver(Player winner);
    void connectSignalsSlots();
    int evaluateBoard(const Player board[6][7]);
    int evaluatePosition(const Player board[6][7], int row, int col, int deltaRow, int deltaCol);
    bool canPlay(const Player board[6][7], int col) const;
    void copyBoard(const Player source[6][7], Player destination[6][7]) const;
    void makeMove(Player board[6][7], int col, Player player);
    bool gameOver(const Player board[6][7]) const;
    int evaluateBoard(const Player board[6][7]) const;
    int evaluatePosition(const Player board[6][7], int row, int col, int deltaRow, int deltaCol) const;
    int minimax(Player board[6][7], int depth, int alpha, int beta, bool isMaximizingPlayer);
    int findEmptyRow(int col) const;
    void setGameMode(GameMode mode);
};

#endif // MAINWINDOW_H
