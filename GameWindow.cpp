// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include <QDesktopWidget>
#include "GameWindow.h"
#include "ui_GameWindow.h"
#include <QDebug>
#include "Card.h"
#include "HoldingStack.h"

CGameWindow::CGameWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::CGameWindow)
{
    // Setup the UI
    ui->setupUi(this);

    // Place the window at the center of the screen
    QDesktopWidget* desktop = QApplication::desktop();
    int screenW = desktop->width();
    int screenH = desktop->height();
    int windowW = this->size().width();
    int windowH = this->size().height();
    this->move((screenW / 2) - (windowW / 2), (screenH / 2) - (windowH / 2));

    // Create a test holding stack
    CHoldingStack* hold = new CHoldingStack(this);
    hold->move(20, 20);
    hold->resize(CCard::getCardScreenSize().width(), windowH - hold->pos().y());
    hold->addCard(new CCard(hold, ECardSymbol::Heart, ECardType::Number, 3));

    qDebug() << "Created CGameWindow";
}

CGameWindow::~CGameWindow()
{
    // Delete the UI
    delete ui;
}

