// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "GameWindow.h"
#include "ui_GameWindow.h"
#include "Card.h"
#include "HoldingStack.h"
#include "Main.h"
#include <QDesktopWidget>
#include <QDebug>
#include <QMessageBox>

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

    // Set background-image
    ui->centralwidget->setStyleSheet("background-image: url(:/assets/table_background.png);");
    ui->centralwidget->resize(1000, 800);

    // Displays the initial score and moves
    ui->score_label->setText("Score: " + QString::number(score));
    ui->move_label->setText("Moves: " + QString::number(moves));

    //ui->undoButton->setStyleSheet("background-color: white");
    ui->undoButton->setEnabled(false);

    // Creation of timer and time as well as connection of timeout signal with updateTime
    timer = new QTimer(this);
    timer->start(1000);
    time = new QTime(0,0);

    // Connection from timer and the CGameWindow
    QObject::connect(timer, &QTimer::timeout, this, &CGameWindow::updateTimer);

    // Connection from the ui menubar with CGameWindow
    QObject::connect(ui->actionQuit, &QAction::triggered, this, &CGameWindow::close);
    QObject::connect(ui->actionAbout, &QAction::triggered, this, &CGameWindow::showAbout);
    QObject::connect(ui->actionNew_Game, &QAction::triggered, this, &CGameWindow::resetGameWindow);
    QObject::connect(ui->actionMusic, &QAction::triggered, this, &CGameWindow::setMusic);
    QObject::connect(ui->actionSound, &QAction::triggered, this, &CGameWindow::setSound);

    // Connection from the ui undo button with CGameWindow
    QObject::connect(ui->undoButton, &QPushButton::clicked, this, &CGameWindow::undo);

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

    QWidget *layout = new QWidget();
    mainGrid = new QGridLayout();
    mainGrid->setSpacing(12);
    layout->setLayout(mainGrid);
    ui->gridLayout->addWidget(layout);
}

void CGameWindow::displayHoldingStack(CHoldingStack* stack, int column)
{
   // stack->resize(CCard::getCardScreenSize().width(), this->size().height() - stack->pos().y());
    stack->setMinimumHeight(CCard::getCardScreenSize().height());
    mainGrid->addWidget(stack, 1, column, 1, 1, Qt::AlignTop);

    // We need a dummy-widget with size policy set to expanding in vertical to force the layout to not change when adding cards
    // to a holdingstack
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setVerticalStretch(1);
    QWidget* stretchDummy = new QWidget();
    stretchDummy->setSizePolicy(policy);
    mainGrid->addWidget(stretchDummy, 2, column);
}

void CGameWindow::displayFinalStack(CFinalStack* final, int column)
{
    //final->resize(CCard::getCardScreenSize().width(), this->size().height() - final->pos().y());
    mainGrid->addWidget(final, 0, column + 3, 1, 1);
    // SetRowMinimumHeight is needed, so that the layout is not changing, when cards are added to the holdingstacks
    mainGrid->setRowMinimumHeight(0, 200);
}

void CGameWindow::displayDrawStack(CDrawStack *draw)
{
    mainGrid->addLayout(draw->getHBoxLayout(), 0, 0, 1, 2);
}

void CGameWindow::incrementMove()
{
    ++moves;
    ui->move_label->setText("Moves: " + QString::number(moves));
}

void CGameWindow::decrementMove()
{
    --moves;
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
    ui->score_label->setText("Score: " + QString::number(score));
}

void CGameWindow::resetGameWindow()
{
    removeAllWidgets(mainGrid);
    time = new QTime(0,0);
    moves = 0;
    emit resetGame();
}

void CGameWindow::setMusic(bool checked)
{
    // Enable music on the sound manager
    CMain::get()->getSoundManager()->setEnableMusic(checked);
}

void CGameWindow::setSound(bool checked)
{
    // Enable sound effects on the sound manager
    CMain::get()->getSoundManager()->setEnableSoundEffects(checked);
}

void CGameWindow::undo()
{
    // Tell the game instance to undo the last move
    CMain::get()->getGameInstance()->undoLastMove();
}

void CGameWindow::removeAllWidgets(QLayout* layout)
{
    QLayoutItem *child;

    // Iterating over all children from the layout
    // if child is layout: call this method again, otherwise delete the child
    // See: https://forum.qt.io/topic/57350/solved-delete-all-items-from-layout/2
    while(layout->count() != 0)
    {
        child = layout->takeAt(0);
        if(child->layout() != 0)
        {
            removeAllWidgets(child->layout());
        }
        else if(child->widget() != 0)
        {
            delete child->widget();
        }
    }
}

void CGameWindow::setUndoButtonEnabled(bool enabled)
{
    ui->undoButton->setEnabled(enabled);
}

CGameWindow::~CGameWindow()
{
    // Delete the UI
    delete ui;
}
