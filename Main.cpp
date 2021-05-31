// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Main.h"
#include <QApplication>
#include "GameWindow.h"

// Singleton instance definition
CMain* CMain::singletonInstance = nullptr;

int CMain::run(int argc, char* argv[])
{
    // Create a new QApplication instance
    QApplication app(argc, argv);

    // Create the game window
    gameWindow = new CGameWindow();
    gameWindow->show();

    // Run the app loop
    return app.exec();
}

CMain* CMain::get()
{
    // Return the singleton instance
    return singletonInstance;
}

CGameWindow* CMain::getGameWindow()
{
    // Return the game window instance
    return gameWindow;
}

// Applications entry point
int main(int argc, char* argv[])
{
    // Create a new CMain and run it
    CMain* main = new CMain();
    return main->run(argc, argv);
}
