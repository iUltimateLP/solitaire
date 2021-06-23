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
    ui->score_label->setText("Score: " + QString::number(score));
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
    QObject::connect(ui->actionNew_Game, &QAction::triggered, CMain::get()->getGameInstance(), &CGame::restartGame);
    QObject::connect(ui->actionNew_Game, &QAction::triggered, this, &CGameWindow::resetGameWindow);
    // Connection from score changing in CGame with CGameWindow
    QObject::connect(CMain::get()->getGameInstance(), &CGame::onScoreChanged, this, &CGameWindow::updateScore);


    // Set the distance between the 3 labels
    ui->labelLayout->setSpacing(25);
    ui->labelLayout->setAlignment(ui->move_label, Qt::AlignRight);
    ui->labelLayout->setAlignment(ui->score_label, Qt::AlignRight);
    ui->labelLayout->setAlignment(ui->time_label, Qt::AlignRight);

    // Style the 3 labels: Timer, Score, Moves and hide the statusbar
    ui->score_label->setStyleSheet("QLabel {color: white}");
    ui->move_label->setStyleSheet("QLabel {color: white}");
    ui->time_label->setStyleSheet("QLabel {color: white}");
    ui->statusbar->hide();
}

void CGameWindow::displayHoldingStack(CHoldingStack* stack, int column)
{
   // stack->resize(CCard::getCardScreenSize().width(), this->size().height() - stack->pos().y());
    ui->gridLayout_3->addWidget(stack, 1, column, 1, 1);
}

void CGameWindow::displayFinalStack(CFinalStack* final, int column)
{
    //final->resize(CCard::getCardScreenSize().width(), this->size().height() - final->pos().y());
    ui->gridLayout_3->addWidget(final, 0, column + 3, 1, 1);
}

void CGameWindow::displayDrawStack(CDrawStack *draw)
{
    ui->gridLayout_3->addLayout(draw->getHBoxLayout(), 0, 0, 1, 2);
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

void CGameWindow::resetGameWindow()
{
    qDebug() << "in reset Game window";
    time = new QTime(0,0);
    moves = 0;
}

CGameWindow::~CGameWindow()
{
    // Delete the UI
    delete ui;
}
