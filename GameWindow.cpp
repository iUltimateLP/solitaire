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

    qDebug() << "Created CGameWindow";

    // Set background-color
    ui->centralwidget->setStyleSheet("background-color:green;");
    ui->centralwidget->resize(1000, 800);

    // Displays the initial score
    ui->score_label->setText("Score is: " + QString::number(score));

    // Creation of timer and time as well as connection of timeout signal with updateTime
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &CGameWindow::updateTimer);
    QObject::connect(ui->actionQuit, &QAction::triggered, this, &CGameWindow::close);
    timer->start(1000);
    time = new QTime(0,0);
}

void CGameWindow::displayHoldingStack(CHoldingStack* stack)
{
    stack->resize(CCard::getCardScreenSize().width(), this->size().height() - stack->pos().y());
    ui->horizontalLayout->addWidget(stack);
}

void CGameWindow::displayFinalStack(CCardStack* final)
{
    final->resize(CCard::getCardScreenSize().width(), this->size().height() - final->pos().y());
    ui->horizontalLayout_3->addWidget(final);
}

void CGameWindow::incrementScore()
{
    ++score;
    ui->score_label->setText("Score: " + QString::number(score));
}

void CGameWindow::updateTimer()
{
    *time = time->addSecs(1);
    qDebug() << time->toString("mm:ss");
    ui->time_label->setText("Timer: " + time->toString("mm:ss"));
}

CGameWindow::~CGameWindow()
{
    // Delete the UI
    delete ui;
}
