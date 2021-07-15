#include "Winscreen.h"
#include "ui_Winscreen.h"

#include "QLabel"
#include <Main.h>

WinScreen::WinScreen(int windowW, int windowH, int windowX, int windowY, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinScreen)
{
    ui->setupUi(this);

    QPixmap winPixmap(":/assets/winscreen.png");
    QLabel* winLabel = new QLabel();
    winLabel->setPixmap(winPixmap.scaled(600,600, Qt::KeepAspectRatio));
 //   winLabel->resize(600, 600);

    ui->verticalLayout->addWidget(winLabel);

    this->setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
  //  this->setWindowFlags(Qt::WindowStaysOnTopHint);
  //  this->setWindowModality(Qt::ApplicationModal);

    this->resize(windowW, windowH-24);
    this->setStyleSheet("background-color: rgba(0, 0, 0, 25);");
    this->setAttribute(Qt::WA_TranslucentBackground);

    // Place the window at the spot of the mainwindow
    this->move(windowX, windowY+60);

    winLabel->setAlignment(Qt::AlignCenter | Qt::AlignTop);

}

void WinScreen::mousePressEvent(QMouseEvent* ev)
{
    CMain::get()->getGameInstance()->restartGame();
    CMain::get()->getGameWindow()->resetGameWindow();
    this->close();
}

WinScreen::~WinScreen()
{
    delete ui;
}
