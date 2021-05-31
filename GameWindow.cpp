#include "GameWindow.h"
#include "ui_GameWindow.h"

CGameWindow::CGameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CGameWindow)
{
    ui->setupUi(this);
}

CGameWindow::~CGameWindow()
{
    delete ui;
}

