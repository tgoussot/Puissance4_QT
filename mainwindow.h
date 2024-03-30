#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>

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
    void playMove(int column); // Ajout de la déclaration de la fonction playMove

public slots:
    void aiMove(); // Ajout de la déclaration de la fonction aiMove

private slots:
    void handleButtonClicked();
    void on_playVsAIButton_clicked();

private:
    Ui::MainWindow *ui;
    Player currentPlayer;
    Player gameBoard[6][7];
    bool isGameOver;
    GameMode gameMode;

    bool gameInProgress;

    // Fonctions de base
    void initializeGame();
    void updateButtonUI(int row, int col);
    int findEmptyRow(int col);
    bool checkVictory(int row, int col);
    void changePlayer();
    void updateUI();
    void gameOver(Player winner);
    void connectSignalsSlots();
    void setGameMode(GameMode mode);
    bool isValidPosition(int row, int col) const;

    // Fonctions liées à l'IA
    int evaluateImmediateThreats();
    int chooseMove();
    int simpleEvaluateBoard();
    int evaluateLinesForPlayer(const Player board[6][7], Player player, int twoInARowScore, int threeInARowScore);
    void simulateMove(Player board[6][7], int col, Player player, Player simulatedBoard[6][7], int &rowPlaced);
    int decideMove();
    void updateBoardForAI();
    bool canPlayInColumn(int column) const;
    bool canPlay(const Player board[6][7], int col) const;
    bool isBoardFull() const;
    void startAI();
    void copyBoard(const Player sourceBoard[6][7], Player destinationBoard[6][7]);

    // Fonctions d'évaluation et de simulation supplémentaires
    int evaluateSequence(const Player board[6][7], Player player, int rowStart, int colStart, int deltaRow, int deltaCol, int twoInARowScore, int threeInARowScore);
    int evaluateLinesForPlayer(const Player board[6][7], Player player, int twoInARowScore, int threeInARowScore, int fourInARowOpenScore);
    int evaluatePosition(const Player board[6][7], Player player);
    int evaluatePotentialSequence(const Player board[6][7], Player player,
                                  int startRow, int startCol,
                                  int deltaRow, int deltaCol,
                                  int twoInRowScore, int threeInRowScore);
};

#endif // MAINWINDOW_H
