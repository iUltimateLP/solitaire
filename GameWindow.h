// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QMainWindow>
#include "Card.h"
#include "HoldingStack.h"
#include "FinalStack.h"
#include "DrawStack.h"
#include <QTimer>
#include <QTime>

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
    void incrementScore(int gameScore);


private slots:
    // Called every second
    void updateTimer();

    // The "About" menu item
    void showAbout();

    // Called when CGame changes its internal score-variable
    void updateScore();

private:
    // Reference to this window
    Ui::CGameWindow* ui;

    // Variable for score and amount of moves
    int score = 0;
    int moves = 0;
    QTimer* timer;
    QTime* time;



};
