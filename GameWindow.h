// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QMainWindow>
#include <Card.h>
#include <HoldingStack.h>
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
    void displayHoldingStack(CHoldingStack* stack);

    // This functions displays the final stacks, is called from game.cpp
    void displayFinalStack(CCardStack* final);

    // Is called after every move
    void incrementMove();
    void incrementScore(int gameScore);

    void showQMessageBox();

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
