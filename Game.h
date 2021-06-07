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
 //   bool isValidMove(Card from, Card to);
 //   void moveCard (Card from, Card to);
    bool hasEnded();
    void setUp();

    //Array of all Cards = deck

/*    CFinalStack* finalStackHeart;
    CFinalStack* finalStackDiamond;
    CFinalStack* finalStackClub;
    CFinalStack* finalStackSpade;
    CHoldingStack* holdingStack1;
    CHoldingStack* holdingStack2;
    CHoldingStack* holdingStack3;
    CHoldingStack* holdingStack4;
    CHoldingStack* holdingStack5;
    CHoldingStack* holdingStack6;
    CHoldingStack* holdingStack7;
    CHoldingStack drawPile;

    Timer time;*/
    QString score;
};
