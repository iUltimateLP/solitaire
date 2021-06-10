// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include <HoldingStack.h>
#include <Card.h>
#include <QTimer>

// Implements the game logic
class CGame : public QObject
{
    Q_OBJECT

public:
    // Constructor
    CGame(QObject* parent = nullptr);
    void moveCard (CCard* cardToDrop, CHoldingStack* srcStack, CCardStack* destStack);
    bool hasEnded();
    void setUp();

private:
    //declaring all necessary stacks
    //final stacks -> TODO: these has to be either other sort of stack or simply cards
    CHoldingStack* finalDiamond;
    CHoldingStack* finalHeart;
    CHoldingStack* finalSpade;
    CHoldingStack* finalClub;

    // List of all holding stacks
    QList<CHoldingStack*> holdingStacks;

    //the deck contains all cards
    QList<CCard*> deck;
};
