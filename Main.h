// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include "GameWindow.h"
#include "Game.h"

// Main program entry class
class CMain : public QObject
{
    Q_OBJECT

public:
    // Constructor
    CMain(QObject* parent = nullptr) : QObject(parent) {}

public:
    // Runs the complete program loop
    int run(int argc, char* argv[]);

    // Singleton getter
    static CMain* get();

    // Returns the GameWindow instance
    CGameWindow* getGameWindow();

    // Returns the Game instance
    CGame* getGameInstance();

private:
    // Singleton instance
    static CMain* singletonInstance;

    // The CGameWindow instance
    CGameWindow* gameWindow;

    // The CGame instance
    CGame* gameInstance;
};
