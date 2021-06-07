// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QMainWindow>
#include <Card.h>

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

    void displayCards(const CCard& c);

private:
    // Reference to this window
    Ui::CGameWindow* ui;
};
