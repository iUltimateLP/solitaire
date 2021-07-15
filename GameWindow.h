// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include "Card.h"
#include "HoldingStack.h"
#include "FinalStack.h"
#include "DrawStack.h"
#include "Winscreen.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CGameWindow; }
QT_END_NAMESPACE

// Implements the main window to be displayed by Qt
class CGameWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor and destructor
    CGameWindow(QWidget* parent = nullptr);
    ~CGameWindow();

    // This functions displays the holding stacks, is called from game.cpp
    void displayHoldingStack(CHoldingStack* stack, int column);

    // This functions displays the final stacks, is called from game.cpp
    void displayFinalStack(CFinalStack* final, int column);

    // This function displays the draw stack, is called from game.cpp
    void displayDrawStack(CDrawStack * draw);

    // Is called after every move
    void incrementMove();
    void decrementMove();
    void incrementScore(int scoreToAdd);

    // Clears the gridlayout where all cards are displayed, needed, when new game is chosen
    void removeAllWidgets(QLayout *layout);

    // Sets whether the undo button is enabled or not
    void setUndoButtonEnabled(bool enabled);

    void showWinScreen();

signals:
    // This signal is send when the new game menu item is chosen, connected with slot from CGame
    void resetGame();

private slots:
    // Called every second
    void updateTimer();

    // The "About" menu item
    void showAbout();


    // The "Music" menu item
    void setMusic(bool checked);

    // The "Sound" menu item
    void setSound(bool checked);

    // The "Undo" button
    void undo();

public slots:
    // Called when CGame changes its internal score-variable
    void updateScore();

    // The "new game" menu item
    void resetGameWindow();

    void closeWindows();

private:
    // Reference to this window
    Ui::CGameWindow* ui;

    // Variable for score and amount of moves
    int score = 0;
    int moves = 0;
    QTimer* timer;
    QTime* time;

    WinScreen* winScreen = nullptr;

    // The main grid layout
    QGridLayout* mainGrid;
    // The layout for the winning score
    QWidget * winningRow;
    QHBoxLayout* winningLayout;
};
