// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include <HoldingStack.h>
#include <Card.h>
#include <QTimer>

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

    // Moves a card from one stack to another, checks first if the move is valid
    void moveCard(CCard* cardToDrop, CCardStack* srcStack, CCardStack* destStack);

    // Evaluates which scoring attribute should be added to the score
    void evaluateScore(CCardStack* srcStack, CCardStack* dstStack);

    // Actually changes the variable score
    void changeScore(int points);
    int getScore();

    // Returns whether the game has ended (all finalStacks have 13 cards)
    bool hasEnded();

signals:
    void onScoreChanged();

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

    int score;
};
