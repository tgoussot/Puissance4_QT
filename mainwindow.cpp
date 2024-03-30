#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QMessageBox>
#include <cstdlib> // Pour std::rand et std::srand
#include <ctime> // Pour std::time

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
    gameInProgress = false;
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
                connect(ui->playVsAIButton, &QPushButton::clicked, this, &MainWindow::on_playVsAIButton_clicked);

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
        if (currentPlayer == Player_Human) {
            color = "red"; // Par exemple, la couleur rouge pour l'humain
        } else if (currentPlayer == Player_AI) {
            color = "yellow"; // La couleur jaune pour l'IA
        }
        button->setStyleSheet(QString("QPushButton { "
                                      "background-color: %1; "
                                      "border-radius: 25px; "
                                      "min-width: 50px; "
                                      "min-height: 50px; "
                                      "max-width: 50px; "
                                      "max-height: 50px; "
                                      "} ").arg(color));
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
    if (gameMode == VersusPlayer) {
        currentPlayer = (currentPlayer == Player1) ? Player2 : Player1;
    } else {
        currentPlayer = (currentPlayer == Player_Human) ? Player_AI : Player_Human;
    }
    qDebug() << "Changement de joueur. Nouveau joueur:" << (currentPlayer == Player_Human || currentPlayer == Player1 ? "Joueur 1" : "Joueur 2 ou IA");
    updateUI();

    if (gameMode == VersusAI && currentPlayer == Player_AI) {
        QTimer::singleShot(1000, this, &MainWindow::aiMove);
    }
}


void MainWindow::updateUI() {
    QString statusMessage;
    if (gameMode == VersusAI) {
        statusMessage = currentPlayer == Player_Human ? "C'est à votre tour de jouer" : "L'IA réfléchit...";
    } else {
        // En mode joueur contre joueur, vous pouvez conserver la logique originale
        statusMessage = currentPlayer == Player1 ? "Joueur 1 à toi de jouer" : "Joueur 2 à toi de jouer";
    }
    ui->statusbar->showMessage(statusMessage);
}


void MainWindow::gameOver(Player winner) {
    QString winnerText;
    if (gameMode == VersusAI) {
        if (winner == Player_Human) {
            winnerText = "Vous avez gagné !";
        } else if (winner == Player_AI) {
            winnerText = "L'IA a gagné.";
        } else {
            winnerText = "Match nul.";
        }
    } else {
        winnerText = (winner == Player1) ? "Joueur 1 a gagné !" : "Joueur 2 a gagné!";
    }

    // Afficher d'abord le message de victoire
    QMessageBox::information(this, "Fin de la partie", winnerText);

    // Puis demander si le joueur veut rejouer
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Game Over", "Voulez-vous rejouer ?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        initializeGame();
    } else {
        this->close();
    }
}
void MainWindow::setGameMode(GameMode mode) {
    qDebug() << "Set game mode to" << (mode == VersusAI ? "Versus AI" : "Versus Player");
    gameMode = mode;
    initializeGame(); // Recommencer ou initialiser le jeu avec le nouveau mode
    // Ajout d'un log pour suivre le joueur actuel après avoir défini le mode de jeu
    qDebug() << "Current player is" << (currentPlayer == Player_Human ? "Human" : "AI");
}


int MainWindow::evaluateImmediateThreats() {
    Player simulatedBoard[6][7]; // Tableau pour stocker l'état simulé du plateau
    int rowPlaced; // Variable pour stocker la ligne où un jeton est placé lors de la simulation

    // Parcours chaque colonne pour vérifier la possibilité de menaces immédiates
    for (int col = 0; col < 7; ++col) {
        if (canPlay(gameBoard, col)) {
            // Simule un coup pour l'IA
            simulateMove(gameBoard, col, Player_AI, simulatedBoard, rowPlaced);
            if (checkVictory(rowPlaced, col)) {
                // Si l'IA peut gagner avec ce coup, retourne cette colonne
                return col;
            }

            // Simule un coup pour le joueur adverse
            simulateMove(gameBoard, col, Player_Human, simulatedBoard, rowPlaced);
            if (checkVictory(rowPlaced, col)) {
                // Si l'adversaire peut gagner avec ce coup, retourne cette colonne pour le bloquer
                return col;
            }
        }
    }

    // Aucune menace immédiate trouvée, retourne -1
    return -1;
}


int MainWindow::chooseMove() {
    // Évalue les menaces immédiates pour gagner ou bloquer
    int immediateThreatColumn = evaluateImmediateThreats();
    if (immediateThreatColumn != -1) {
        // Si une menace immédiate est trouvée, joue dans cette colonne
        return immediateThreatColumn;
    }

    // Liste pour stocker les indices des colonnes jouables
    QVector<int> playableColumns;
    for (int col = 0; col < 7; ++col) {
        if (canPlay(gameBoard, col)) {
            playableColumns.append(col);
        }
    }

    // Si aucune colonne n'est jouable, retourne -1
    if (playableColumns.isEmpty()) {
        return -1;
    }

    // Sélection aléatoire d'une colonne parmi les colonnes jouables
    int randomIndex = rand() % playableColumns.size(); // Assurez-vous d'avoir appelé srand() quelque part dans votre code pour initialiser le générateur de nombres aléatoires
    return playableColumns[randomIndex];
}

int MainWindow::simpleEvaluateBoard() {
    int score = 0;

    // Définir les scores pour différentes situations
    const int TWO_IN_A_ROW_SCORE = 10;
    const int THREE_IN_A_ROW_SCORE = 100;

    // Évaluation pour le joueur actuel
    score += evaluateLinesForPlayer(gameBoard, currentPlayer, TWO_IN_A_ROW_SCORE, THREE_IN_A_ROW_SCORE);

    // Évaluation pour l'adversaire
    Player opponent = (currentPlayer == Player1) ? Player2 : Player1;
    score -= evaluateLinesForPlayer(gameBoard, opponent, TWO_IN_A_ROW_SCORE, THREE_IN_A_ROW_SCORE);

    return score;
}

int MainWindow::evaluateLinesForPlayer(const Player board[6][7], Player player, int twoInARowScore, int threeInARowScore) {
    int score = 0;

    // Évaluer les alignements horizontaux
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 5; ++col) { // Seulement jusqu'à 5 pour permettre la vérification de séquences
            score += evaluateSequence(board, player, row, col, 0, 1, twoInARowScore, threeInARowScore);
        }
    }

    // Évaluer les alignements verticaux
    for (int col = 0; col < 7; ++col) {
        for (int row = 0; row < 4; ++row) { // Seulement jusqu'à 4 pour permettre la vérification de séquences
            score += evaluateSequence(board, player, row, col, 1, 0, twoInARowScore, threeInARowScore);
        }
    }

    // Évaluer les diagonales montantes
    for (int row = 3; row < 6; ++row) {
        for (int col = 0; col < 5; ++col) { // Adapté pour les diagonales montantes
            score += evaluateSequence(board, player, row, col, -1, 1, twoInARowScore, threeInARowScore);
        }
    }

    // Évaluer les diagonales descendantes
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 5; ++col) { // Adapté pour les diagonales descendantes
            score += evaluateSequence(board, player, row, col, 1, 1, twoInARowScore, threeInARowScore);
        }
    }

    return score;
}

int MainWindow::evaluateSequence(const Player board[6][7], Player player, int rowStart, int colStart, int deltaRow, int deltaCol, int twoInARowScore, int threeInARowScore) {
    int count = 0;
    for (int i = 0; i < 3; ++i) { // Vérifier les séquences de 3 pour potentiellement compter des séquences de 2 et 3
        int row = rowStart + i * deltaRow;
        int col = colStart + i * deltaCol;
        if (row >= 0 && row < 6 && col >= 0 && col < 7 && board[row][col] == player) {
            ++count;
        } else {
            // Si un jeton non-joueur est trouvé dans la séquence, réinitialiser le comptage
            count = 0;
            break;
        }
    }

    if (count == 3) {
        return threeInARowScore;
    } else if (count == 2) {
        return twoInARowScore;
    }

    return 0;
}

void MainWindow::simulateMove(Player board[6][7], int col, Player player, Player simulatedBoard[6][7], int &rowPlaced) {
    // Copie du plateau actuel vers le plateau simulé
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            simulatedBoard[i][j] = board[i][j];
        }
    }

    // Trouver la première rangée vide depuis le bas dans la colonne spécifiée
    rowPlaced = -1; // -1 signifie qu'aucun coup n'a pu être simulé (colonne pleine)
    for (int row = 5; row >= 0; --row) {
        if (simulatedBoard[row][col] == NoPlayer) {
            simulatedBoard[row][col] = player;
            rowPlaced = row;
            break;
        }
    }
}

int MainWindow::decideMove() {
    int bestMove = -1; // Initialiser à -1, signifiant aucun coup choisi
    int highestScore = INT_MIN; // Initialiser à la plus petite valeur pour commencer la comparaison

    // Évaluer chaque colonne comme un coup potentiel
    for (int col = 0; col < 7; ++col) {
        if (canPlay(gameBoard, col)) {
            // Créer un tableau simulé basé sur le jeu actuel pour simuler le coup
            Player simulatedBoard[6][7];
            int rowPlaced; // Ligne où le jeton sera placé lors de la simulation
            simulateMove(gameBoard, col, currentPlayer, simulatedBoard, rowPlaced);

            if (rowPlaced != -1) { // Si le coup est possible
                // Évaluer le plateau après le coup simulé pour ce joueur
                // Note: Assurez-vous que evaluateImmediateThreats et simpleEvaluateBoard utilisent gameBoard directement
                int moveScore = evaluateImmediateThreats() + simpleEvaluateBoard();

                // Choisir le meilleur coup basé sur le score
                if (moveScore > highestScore) {
                    highestScore = moveScore;
                    bestMove = col;
                }
            }
        }
    }

    // Ajouter de l'aléatoire si nécessaire pour éviter la prévisibilité
    if (bestMove == -1 || (rand() % 10) < 2) { // Par exemple, 20% de chance de choisir un coup aléatoire
        do {
            bestMove = rand() % 7;
        } while (!canPlay(gameBoard, bestMove));
    }

    return bestMove;
}

void MainWindow::updateBoardForAI() {
    // Décider du coup à jouer
    int chosenMove = decideMove();

    if (chosenMove != -1) { // Vérifier si un coup valide a été trouvé
        int rowToPlay = findEmptyRow(chosenMove); // Trouver la première ligne disponible dans la colonne
        if (rowToPlay != -1) { // Si une ligne est disponible
            gameBoard[rowToPlay][chosenMove] = currentPlayer; // Mettre à jour le plateau avec le coup de l'IA

            // Mettre à jour l'interface utilisateur pour refléter le coup joué
            updateButtonUI(rowToPlay, chosenMove);

            // Vérifier si ce coup cause une victoire ou un match nul
            if (checkVictory(rowToPlay, chosenMove)) {
                gameOver(currentPlayer); // Gérer la fin du jeu si l'IA gagne
            } else if (isBoardFull()) {
                gameOver(NoPlayer); // Gérer le match nul
            } else {
                changePlayer(); // Passer au joueur suivant si le jeu continue
            }
        }
    }
}

bool MainWindow::canPlayInColumn(int column) const {
    // Vérifier si la cellule supérieure de la colonne est vide
    return gameBoard[0][column] == NoPlayer;
}

bool MainWindow::canPlay(const Player board[6][7], int col) const {
    // Vérifie si la case supérieure de la colonne est vide (NoPlayer),
    // ce qui indique qu'un coup est possible dans cette colonne.
    return board[0][col] == NoPlayer;
}

bool MainWindow::isBoardFull() const {
    for (int col = 0; col < 7; ++col) {
        // Si une colonne peut jouer, cela signifie que le plateau n'est pas encore plein.
        if (canPlay(gameBoard, col)) {
            return false; // Le plateau n'est pas plein.
        }
    }
    return true; // Toutes les colonnes sont pleines, donc le plateau est plein.
}

void MainWindow::playMove(int column) {
    int row = findEmptyRow(column);
    if(row != -1) {
        qDebug() << "Coup joué à la position: (" << row << "," << column << ")";
        gameBoard[row][column] = currentPlayer;
        updateButtonUI(row, column);

        if (checkVictory(row, column)) {
            qDebug() << "Victoire détectée pour le joueur:" << currentPlayer;
            gameOver(currentPlayer);
        } else if (isBoardFull()) {
            qDebug() << "Plateau plein. Match nul.";
            gameOver(NoPlayer);
        } else {
            changePlayer();
        }
    } else {
        qDebug() << "La colonne est pleine. Coup non joué.";
    }
}


void MainWindow::startAI() {
    // Configure le jeu pour le mode IA
    setGameMode(VersusAI);
    // Peut-être déclencher un premier coup de l'IA si elle doit commencer
    aiMove();
}

void MainWindow::aiMove() {
    qDebug() << "aiMove() called";
    int column = decideMove();
    if (column != -1) {
        qDebug() << "IA joue dans la colonne:" << column;
        playMove(column); // Cette fonction met à jour le plateau et change le joueur
    } else {
        qDebug() << "Aucun coup possible pour l'IA. Vérifiez la logique de decideMove.";
    }
}


void MainWindow::on_playVsAIButton_clicked()
{
    if (!gameInProgress) {
        setGameMode(VersusAI);
        currentPlayer = Player_Human;
        initializeGame();
        gameInProgress = true; // Ajoutez un membre booléen gameInProgress à votre classe
    } else {
        // Peut-être afficher un message disant que le jeu est déjà en cours
    }
}

