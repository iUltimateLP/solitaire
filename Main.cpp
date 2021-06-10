// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Main.h"
#include <QApplication>
#include <QDebug>

// Singleton instance definition
CMain* CMain::singletonInstance = nullptr;

int CMain::run(int argc, char* argv[])
{
    // Create a new QApplication instance
    QApplication app(argc, argv);

    // Assign the singleton instance
    singletonInstance = this;

    // Create the game window
    gameWindow = new CGameWindow();
    gameWindow->show();

    // Create the game instance
    gameInstance = new CGame();

    qDebug() << "Entering app loop";

    // Run the app loop
    return app.exec();
}

CMain* CMain::get()
{
    // Return the singleton instance
    return singletonInstance;
}

CGameWindow* CMain::getGameWindow() const
{
    // Return the game window instance
    return gameWindow;
}

CGame* CMain::getGameInstance() const
{
    // Returns the game instance
    return gameInstance;
}

// Applications entry point
int main(int argc, char* argv[])
{
    // Create a new CMain and run it
    CMain* main = new CMain();
    return main->run(argc, argv);
}
