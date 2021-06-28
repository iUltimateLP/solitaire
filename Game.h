// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include "HoldingStack.h"
#include "FinalStack.h"
#include "Card.h"
#include <QTimer>
#include "DrawStack.h"

// Contains the scoring attributes for the game
namespace GameScoringAttributes
{
    // DrawStack is empty after a move and has to be recycled
    static int RECYCLING_DRAW_PILE = -100;

    // Moving a card form the DrawStack to a HoldingStack
    static int WASTE_PILE_TO_TABLEAU = 5;

    // Moving a card from the DrawStack to a FinalStack
    static int WASTE_PILE_TO_FOUNDATION = 10;

    // Moving a card from a HoldingStack to a FinalStack
    static int TABLEAU_TO_FOUNDATION = 10;

    // Flipping a card form a HoldingStack when previous card is moved
    static int TURN_OVER_TABLEAU_CARD = 5;

    // Moving a card back from a FinalStack to a HoldingStack
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

    // Moves a card, when clicked and a movement is possible
    bool moveCard(CCard* cardToDrop, CCardStack* srcStack);

    // Evaluates which scoring attribute should be added to the score
    void evaluateScore(CCardStack* srcStack, CCardStack* destStack);

    // Actually changes the variable score
    void changeScore(int points);
    int getScore();

    // Returns whether the game has ended (all finalStacks have 13 cards)
    bool hasEnded();

signals:
    // The "new game" menu item
    void onScoreChanged();


public slots:
    void restartGame();

private:
    CDrawStack* drawStack;

    // The final stacks at the top
    QList<CFinalStack*> finalStacks;
    QList<CHoldingStack*> holdingStacks;
    CFinalStack* finalDiamond;
    CFinalStack* finalHeart;
    CFinalStack* finalSpade;
    CFinalStack* finalClub;

    // The deck contains all cards in the whole game
    QList<CCard*> deck;

    int score = 0;
};
