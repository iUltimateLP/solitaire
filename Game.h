// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>

// Implements the game logic
class CGame : public QObject
{
    Q_OBJECT

public:
    // Constructor
    CGame(QObject* parent = nullptr);
};
