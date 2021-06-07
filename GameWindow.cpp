// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include <QDesktopWidget>
#include "GameWindow.h"
#include "ui_GameWindow.h"
#include <QDebug>


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

    //Set background-color
    ui->centralwidget->setStyleSheet("background-color:green;");
    ui->centralwidget->resize(1000, 800);

    //set the 4 symbols to the finalstacks;

}


void CGameWindow::displayCards(const CCard& c)
{
    //TODO: choose correct label: switch case or variable
    ui->label_2->setPixmap(c.getPixmap());
    ui->label_2->resize(ui->label_2->pixmap()->size());
}

CGameWindow::~CGameWindow()
{
    // Delete the UI
    delete ui;
}

