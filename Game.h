// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include <HoldingStack.h>
#include <Card.h>
#include <QTimer>

namespace GameScoringAttributes
{
    static int RECYCLING_DRAW_PILE = -100;
    static int WASTE_PILE_TO_TABLEAU = 5;
    static int WASTE_PILE_TO_FOUNDATION = 10;
    static int TABLEAU_TO_FOUNDATION = 10;
    static int TURN_OVER_TABLEAU_CARD = 5;
    static int FOUNDATION_TO_TABLEAU = -15;
}

// Implements the game logic
class CGame : public QObject
{
    Q_OBJECT

public:
    // Constructor
    CGame(QObject* parent = nullptr);

    // The initial setup of the game board, which shuffles the card deck, fills all card stacks
    // and calls the GameWindow to display everything
    void setUp();

    // Moves a card from one stack to another, checks first if the move is valid
    void moveCard(CCard* cardToDrop, CCardStack* srcStack, CCardStack* destStack);

    // Returns whether the game has ended (all finalStacks have 13 cards)
    bool hasEnded();

private:
    // The final stacks at the top
    CHoldingStack* finalDiamond;
    CHoldingStack* finalHeart;
    CHoldingStack* finalSpade;
    CHoldingStack* finalClub;

    // List of all holding stacks
    QList<CHoldingStack*> holdingStacks;

    // The deck contains all cards in the whole game
    QList<CCard*> deck;
};
