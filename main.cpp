#include "GameWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CGameWindow w;
    w.show();
    return a.exec();
}
