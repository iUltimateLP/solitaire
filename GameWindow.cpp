// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include <QDesktopWidget>
#include "GameWindow.h"
#include "ui_GameWindow.h"
#include <QDebug>
#include <QMessageBox>
#include "Card.h"
#include "HoldingStack.h"
#include "Main.h"

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

    qDebug() << "Created CGameWindow";

    // Set background-color
    ui->centralwidget->setStyleSheet("background-image: url(:/assets/table_background.png);");
    ui->centralwidget->resize(1000, 800);

    // Displays the initial score and moves
    ui->score_label->setText("Score is: " + QString::number(score));
    ui->move_label->setText("Moves: " + QString::number(moves));

    // Creation of timer and time as well as connection of timeout signal with updateTime
    timer = new QTimer(this);
    timer->start(1000);
    time = new QTime(0,0);
    // Connection from timer and the CGameWindow
    QObject::connect(timer, &QTimer::timeout, this, &CGameWindow::updateTimer);

    // Connection from the ui menubar with CGameWindow
    QObject::connect(ui->actionQuit, &QAction::triggered, this, &CGameWindow::close);
    QObject::connect(ui->actionAbout, &QAction::triggered, this, &CGameWindow::showAbout);
    // Connection from score changing in CGame with CGameWindow
    QObject::connect(CMain::get()->getGameInstance(), &CGame::onScoreChanged, this, &CGameWindow::updateScore);
}

void CGameWindow::displayHoldingStack(CHoldingStack* stack)
{
   // stack->resize(CCard::getCardScreenSize().width(), this->size().height() - stack->pos().y());
    ui->horizontalLayout->addWidget(stack);
}

void CGameWindow::displayFinalStack(CCardStack* final)
{
    final->resize(CCard::getCardScreenSize().width(), this->size().height() - final->pos().y());
    ui->horizontalLayout_3->addWidget(final);
}

void CGameWindow::incrementMove()
{
    ++moves;
    ui->move_label->setText("Moves: " + QString::number(moves));
}

void CGameWindow::incrementScore(int gameScore)
{
    score += gameScore;
    ui->score_label->setText("Score: " + QString::number(score));
}

void CGameWindow::updateTimer()
{
    *time = time->addSecs(1);
    qDebug() << time->toString("mm:ss");
    ui->time_label->setText("Timer: " + time->toString("mm:ss"));
}


void CGameWindow::showAbout()
{
    // Simply show a message box
    QMessageBox msgBox(QMessageBox::Information, "About Solitaire", "Solitaire v1.0\n\
Written in C++ and Qt.\n\n\
Created by Annie Berend (5033782) and Jonathan Verbeek (5058288)");
    msgBox.exec();
}

void CGameWindow::updateScore()
{
    score = CMain::get()->getGameInstance()->getScore();
}

CGameWindow::~CGameWindow()
{
    // Delete the UI
    delete ui;
}
