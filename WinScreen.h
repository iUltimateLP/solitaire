// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QWidget>

namespace Ui {class WinScreen; }

// This UI widget is the screen displayed when winning the game
class CWinScreen : public QWidget
{
    Q_OBJECT

public:
    // Constructor and destructor
    explicit CWinScreen(int score, int moves, QString time, QWidget *parent = nullptr);
    ~CWinScreen();

private:
    // Overwritten mouse events
    void mousePressEvent(QMouseEvent* ev) override;

    // Overwritten close event
    void closeEvent(QCloseEvent* ev) override;

private:
    Ui::WinScreen *ui;
};
