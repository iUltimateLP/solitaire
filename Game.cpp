// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Game.h"
#include <QDebug>

CGame::CGame(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Created CGame";
}
