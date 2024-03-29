#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentPlayer(Player1), isGameOver(false)
{
    ui->setupUi(this);
    initializeGame();
    connectSignalsSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeGame() {
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            gameBoard[row][col] = NoPlayer;
            QString buttonName = QString("pushButton_%1_%2").arg(row).arg(col);
            QPushButton* button = ui->centralwidget->findChild<QPushButton*>(buttonName);
            if (button) {
                button->setStyleSheet("QPushButton { "
                                      "background-color: white; "
                                      "border-radius: 25px; "
                                      "min-width: 50px; "
                                      "min-height: 50px; "
                                      "max-width: 50px; "
                                      "max-height: 50px; "
                                      "} ");
            }
        }
    }
    isGameOver = false;

    // Définir le joueur initial en fonction du mode de jeu
    if (gameMode == VersusAI) {
        currentPlayer = Player_Human; // Si le jeu est contre l'IA, commencez toujours par l'humain
    } else {
        // Ici, vous pouvez choisir aléatoirement ou fixer qui commence lorsque c'est joueur vs joueur
        // Exemple : commencer par Player1
        currentPlayer = Player1;
    }
    updateUI();
}

void MainWindow::connectSignalsSlots() {
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            QString buttonName = QString("pushButton_%1_%2").arg(row).arg(col);
            QPushButton* button = ui->centralwidget->findChild<QPushButton*>(buttonName);
            if (button) {
                connect(button, &QPushButton::clicked, this, &MainWindow::handleButtonClicked);
            }
        }
    }
}

void MainWindow::handleButtonClicked() {
    if (isGameOver) {
        initializeGame();
        return;
    }

    QPushButton* buttonClicked = qobject_cast<QPushButton*>(sender());
    if (buttonClicked) {
        QStringList parts = buttonClicked->objectName().split('_');
        // int row = parts.at(1).toInt();
        int col = parts.at(2).toInt();

        int emptyRow = findEmptyRow(col);
        if (emptyRow == -1) return;

        gameBoard[emptyRow][col] = currentPlayer;
        updateButtonUI(emptyRow, col);
        if (checkVictory(emptyRow, col)) {
            gameOver(currentPlayer);
        } else {
            changePlayer();
        }
    }
}

int MainWindow::findEmptyRow(int col) {
    for (int row = 5; row >= 0; --row) {
        if (gameBoard[row][col] == NoPlayer) {
            return row;
        }
    }
    return -1;
}

void MainWindow::updateButtonUI(int row, int col) {
    QString buttonName = QString("pushButton_%1_%2").arg(row).arg(col);
    QPushButton* button = ui->centralwidget->findChild<QPushButton*>(buttonName);
    if (button) {
        QString color = (currentPlayer == Player1) ? "red" : "yellow";
        button->setStyleSheet(QString("QPushButton { "
                                      "background-color: %1; "
                                      "border-radius: 25px; "
                                      "min-width: 50px; "
                                      "min-height: 50px; "
                                      "max-width: 50px; "
                                      "max-height: 50px; "
                                      "} ").arg(color, "#dddddd"));
    }
}


bool MainWindow::checkVictory(int row, int col) {
    Player current = gameBoard[row][col];

    // Check if the current player is NoPlayer, then there's no point checking for victory
    if (current == NoPlayer) {
        return false;
    }

    // Check horizontal
    int count = 0;
    for (int c = 0; c < 7; ++c) {
        count = (gameBoard[row][c] == current) ? count+1 : 0;
        if (count >= 4) return true;
    }

    // Check vertical
    count = 0;
    for (int r = 0; r < 6; ++r) {
        count = (gameBoard[r][col] == current) ? count+1 : 0;
        if (count >= 4) return true;
    }

    // Check diagonal (top-left to bottom-right)
    count = 0;
    for (int offset = -3; offset <= 3; ++offset) {
        int r = row + offset;
        int c = col + offset;
        if (r >= 0 && r < 6 && c >= 0 && c < 7) {
            count = (gameBoard[r][c] == current) ? count+1 : 0;
        } else {
            count = 0;
        }
        if (count >= 4) return true;
    }

    // Check diagonal (bottom-left to top-right)
    count = 0;
    for (int offset = -3; offset <= 3; ++offset) {
        int r = row + offset;
        int c = col - offset;
        if (r >= 0 && r < 6 && c >= 0 && c < 7) {
            count = (gameBoard[r][c] == current) ? count+1 : 0;
        } else {
            count = 0;
        }
        if (count >= 4) return true;
    }

    return false;
}

void MainWindow::changePlayer() {
    currentPlayer = (currentPlayer == Player1) ? Player2 : Player1;
    updateUI();
}


void MainWindow::updateUI() {
    QString statusMessage = currentPlayer == Player1 ? "Joueur 1 à toi de jouer" : "Joueur 2 à toi de jouer";
    ui->statusbar->showMessage(statusMessage);
}

void MainWindow::gameOver(Player winner) {
    QString winnerText = (winner == Player1) ? "Joueur 1 a gagné !" : "Joueur 2 a agné!";
    QMessageBox::information(this, "Game Over", winnerText);

    // Proposez de recommencer ou de quitter le jeu
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Game Over", "Rejouez ?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        initializeGame();
    } else {
        this->close();
    }
}

int MainWindow::evaluateBoard(const Player board[6][7]) {
    int score = 0;

    // Scores basés sur le nombre de jetons alignés
    const int FOUR_IN_A_ROW = 100000;
    const int THREE_IN_A_ROW = 100;
    const int TWO_IN_A_ROW = 10;

    // Vérifier les alignements horizontaux
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            score += evaluatePosition(board, row, col, 0, 1);
        }
    }

    // Vérifier les alignements verticaux
    for (int col = 0; col < 7; ++col) {
        for (int row = 0; row < 3; ++row) {
            score += evaluatePosition(board, row, col, 1, 0);
        }
    }

    // Vérifier les alignements diagonaux montants
    for (int row = 3; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            score += evaluatePosition(board, row, col, -1, 1);
        }
    }

    // Vérifier les alignements diagonaux descendants
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            score += evaluatePosition(board, row, col, 1, 1);
        }
    }

    return score;
}

int MainWindow::evaluatePosition(const Player board[6][7], int row, int col, int deltaRow, int deltaCol) {
    int humanScore = 0;
    int aiScore = 0;

    for (int i = 0; i < 4; i++) {
        if (board[row][col] == Player_Human) {
            humanScore++; // Incrementer pour chaque jeton du joueur humain
        } else if (board[row][col] == Player_AI) {
            aiScore++; // Incrementer pour chaque jeton de l'IA
        }
        row += deltaRow;
        col += deltaCol;
    }

    // Attribuer un score basé sur les alignements
    if (humanScore == 4) {
        return -FOUR_IN_A_ROW;
    } else if (aiScore == 4) {
        return FOUR_IN_A_ROW;
    } else if (aiScore == 3 && humanScore == 0) {
        return THREE_IN_A_ROW;
    } else if (aiScore == 2 && humanScore == 0) {
        return TWO_IN_A_ROW;
    } else if (humanScore == 3 && aiScore == 0) {
        return -THREE_IN_A_ROW;
    } else if (humanScore == 2 && aiScore == 0) {
        return -TWO_IN_A_ROW;
    }

    return 0;
}



bool MainWindow::canPlay(const Player board[6][7], int col) const {
    // Vérifie si la cellule supérieure de la colonne est vide.
    return board[0][col] == Player_None;
}

void MainWindow::makeMove(Player board[6][7], int col, Player player) {
    // Parcourir les cellules de la colonne de bas en haut
    for (int row = 5; row >= 0; --row) {
        if (board[row][col] == Player_None) {
            board[row][col] = player;
            break; // Arrêter après avoir placé le jeton
        }
    }
}

void MainWindow::setGameMode(GameMode mode) {
    gameMode = mode;
    initializeGame(); // Recommencez ou initialisez le jeu avec le nouveau mode
}
