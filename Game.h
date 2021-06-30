// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include "HoldingStack.h"
#include "FinalStack.h"
#include "Card.h"
#include <QTimer>
#include "DrawStack.h"
#include <QTextStream>

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

// A transaction is an action by the user, such as move a card
struct Transaction
{
    enum TransactionType {
        StackToStack,
        DrawFromDrawStack
    } type;

    CCardStack* stack1;
    CCardStack* stack2;
    QList<CCard*> cards;

    QString toString() {
        QString str;
        QTextStream stream(&str);

        // Format type and symbol strings
        QString typeStr;
        switch (type) {
            case TransactionType::StackToStack: typeStr = "Stack<->Stack";  break;
            case TransactionType::DrawFromDrawStack:  typeStr = "DrawFromDrawStack";    break;
        }

        // Create final string
        stream << "(Transaction " << this << " type=" << typeStr << " stack1=" << stack1 << " stack2=" << stack2;
        return str;
    }
};

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

    // Registers a new transaction
    void addTransaction(Transaction newTransaction);

    // Undoes the last move
    void undoLastMove();

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

    // List of latest transactions the player has made
    QList<Transaction> transactions;

    int score = 0;
};
