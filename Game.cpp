// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Game.h"
#include "Main.h"
#include "GameWindow.h"
#include "HoldingStack.h"
#include "CardStack.h"
#include "FinalStack.h"
#include <QDebug>

// Set the static variables
int CGame::MaxUndoSteps = 3;

CGame::CGame(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Created CGame";

    // Initially set up the game
    setUp();

    // Connection, when New Game menu item is pressed
    QObject::connect(CMain::get()->getGameWindow(), &CGameWindow::resetGame, this, &CGame::restartGame);

    // Connection from score changing in CGame with CGameWindow
    QObject::connect(this, &CGame::onScoreChanged, CMain::get()->getGameWindow(), &CGameWindow::updateScore);
}

void CGame::setUp()
{
    const ECardSymbol symbolVector[] = {Club, Heart, Diamond, Spade};
    const ECardType typeVector[] = {Jack, Queen, King, Ace};

    // Filling the deck initially
    for(ECardSymbol symbol : symbolVector)
    {
        // Create all number cards
        for (int i = 2; i < 11; ++i)
        {
            CCard* newCard = new CCard(CMain::get()->getGameWindow(), symbol, ECardType::Number, i);
            deck.push_back(newCard);
        }

        // Create all special cards
        for(ECardType type : typeVector)
        {
            CCard* newCard = new CCard(CMain::get()->getGameWindow(), symbol, type, 0);
            deck.push_back(newCard);
        }
    }

    // Seed the random number generator and shuffle the carddeck
    std::srand(time(0));
    std::random_shuffle(deck.begin(), deck.end());

    // Clear the holding- and finalStacks before creating them is necessary for the "new game"-action
    holdingStacks.clear();
    finalStacks.clear();

    // Creation of the 4 final stacks, these are initially empty
    finalStacks.append(new CFinalStack(CMain::get()->getGameWindow(), ECardSymbol::Heart));
    finalStacks.append(new CFinalStack(CMain::get()->getGameWindow(), ECardSymbol::Diamond));
    finalStacks.append(new CFinalStack(CMain::get()->getGameWindow(), ECardSymbol::Club));
    finalStacks.append(new CFinalStack(CMain::get()->getGameWindow(), ECardSymbol::Spade));

    // Call the CGameWindow to display the initial state of the finalStacks
    for(int i = 0; i < 4; ++i)
    {
        CMain::get()->getGameWindow()->displayFinalStack(finalStacks[i], i);

        // Hook their onCardsChange signal so we can detect wins
        QObject::connect(finalStacks[i], &CFinalStack::onCardsChanged, this, &CGame::checkHasEnded);
    }

    // Create seven holding stacks and fill with cards
    for(int i = 0; i < 7; ++i)
    {
        // Add a new holding stack
        CHoldingStack* newHoldingStack = new CHoldingStack();
        holdingStacks.push_back(newHoldingStack);

        // Display the new holding stack
        CMain::get()->getGameWindow()->displayHoldingStack(newHoldingStack, i);

        // Every n'th holding stack has n+1 cards
        for(int j = 0; j < i+1; ++j)
        {
            // Add the next card from the deck
            holdingStacks[i]->addCard(deck.front());
            // Flip the card if it's not the top card in the stack
            deck.front()->setCardFlipped(j >= i);

            // Remove it from the deck list
            deck.pop_front();
        }
    }

    // Create the drawStack and adding the left cards of the deck to it
    drawStack = new CDrawStack(CMain::get()->getGameWindow());

    for(int i = 0; i < deck.size(); ++i)
    {
        drawStack->addCard(deck[i]);
    }

    // Call the CGameWindow to display the initial state of the drawStack and clear the deck
    CMain::get()->getGameWindow()->displayDrawStack(drawStack);
    deck.clear();

    // Clear the transaction buffer
    transactions.clear();
}

bool CGame::moveCard(CCard* cardToDrop, CCardStack* srcStack)
{
    // The found stack where we can add the card to
    CCardStack* foundStack = nullptr;

    // We need a list, if the cardToDrop is not the topCard
    QList<CCard*> cardsToMove;

    // Iterate over the final stacks and check if card can be dropped
    for (CFinalStack* final : finalStacks)
    {
        // If the card fits on a final stack, it has to be the top card as only a single card can be dropped there
        if (final->canDropCard(cardToDrop) && cardToDrop == srcStack->getTopCard() && final != srcStack)
        {
            // Add the card to the cardsToMove list and assign the found stack
            cardsToMove.push_back(cardToDrop);
            foundStack = final;

            // Increment the amount of steps
            CMain::get()->getGameWindow()->incrementMove();
            break;
        }
    }

    // If we didn't find a suitable stack yet
    if (!foundStack)
    {
        // Try to find a suitable holding stack
        for (CHoldingStack* holding : holdingStacks)
        {
            // Can it be dropped?
            if (holding->canDropCard(cardToDrop) && holding != srcStack)
            {
                // Add it to the list and mark that we found it
                cardsToMove.push_back(cardToDrop);
                foundStack = holding;

                // If the source stack is a holding stack too...
                if (dynamic_cast<CHoldingStack*>(srcStack) != NULL)
                {
                    // ...get all cards above this one and add them too
                    for (CCard* card : dynamic_cast<CHoldingStack*>(srcStack)->getCardsAbove(cardToDrop))
                    {
                        // Ignore the card to drop
                        if (card == cardToDrop) continue;
                        cardsToMove.push_back(card);
                    }
                }

                // Increment the amount of steps
                CMain::get()->getGameWindow()->incrementMove();
                break;
            }
        }
    }

    // Now check if a stack was found
    if (foundStack)
    {
        // Register a new transaction
        Transaction t;

        // If the source stack was a DrawStack, we need to flag that in the type
        if(dynamic_cast<CDrawStack*>(srcStack) != NULL)
            t.type = Transaction::TransactionType::DrawToStack;
        else
            t.type = Transaction::TransactionType::StackToStack;

        t.stack1 = srcStack;
        t.stack2 = foundStack;
        t.cards = cardsToMove;
        t.scoreBefore = this->score;
        t.flipCardBefore = (srcStack->getTopCard() != nullptr ? srcStack->getTopCard()->getFlipped() : false);

        // Go over all cards to move from front to back
        while (cardsToMove.size() > 0)
        {
            srcStack->removeCard(cardsToMove.front());
            foundStack->addCard(cardsToMove.front());
            cardsToMove.pop_front();
        }

        // Change the score
        evaluateScore(srcStack, foundStack);

        // Store the score after evaluation in the transaction
        t.scoreAfter = this->score;

        // Add the transaction
        this->addTransaction(t);

        return true;
    }

    return false;
}

void CGame::evaluateScore(CCardStack *srcStack, CCardStack *dstStack)
{
    // Increment the score with the suitable attribute
    if(dynamic_cast<CHoldingStack*>(srcStack) != NULL && dynamic_cast<CFinalStack*>(dstStack) != NULL)
    {
        // Points for moving a card from a HoldingStack to a FinalStack
        addScore(GameScoringAttributes::TABLEAU_TO_FOUNDATION);
    }
    else if (dynamic_cast<CFinalStack*>(srcStack) != NULL && dynamic_cast<CHoldingStack*>(dstStack) != NULL)
    {
        // Minus points if card is moved from FinalStack back to a HoldingStack
        addScore(GameScoringAttributes::FOUNDATION_TO_TABLEAU);
    }
    else if (dynamic_cast<CDrawStack*>(srcStack) != NULL && dynamic_cast<CHoldingStack*>(dstStack) != NULL)
    {
        // Points for moving a card from the DrawStack to a HoldingStack
        addScore(GameScoringAttributes::WASTE_PILE_TO_TABLEAU);
    }
    else if (dynamic_cast<CDrawStack*>(srcStack) != NULL && dynamic_cast<CFinalStack*>(dstStack) != NULL)
    {
        // Points for moving a card directly from the DrawStack to a FinalStack
        addScore(GameScoringAttributes::WASTE_PILE_TO_FOUNDATION);
    }
}

void CGame::addScore(int points)
{
    // Update the score and make sure it does not go below zero
    score = qMax<int>(0, score + points);

    // Emit the signal
    emit onScoreChanged();
}

int CGame::getScore()
{
    return score;
}

void CGame::checkHasEnded()
{
    // Check if all final stacks got 13 cards
    bool hasEnded = true;
    for (CFinalStack* finalStack : finalStacks)
    {
        hasEnded &= (finalStack->getNumCards() == 13);
    }

    if (hasEnded)
    {
        CMain::get()->getGameWindow()->showWinScreen();
        this->isWon = true;
        // We won!
        qDebug() << "Won!";
    }
    else
    {
        // Not won yet!
        qDebug() << "Not won!";
    }
}

bool CGame::hasEnded()
{
    return this->isWon;
}

void CGame::addTransaction(Transaction newTransaction)
{
    // Remove transactions from the front to stay at MaxUndoSteps
    int transactionsToRemove = transactions.length() - CGame::MaxUndoSteps + 1;
    if (transactionsToRemove > 0) {
        for (int i = 0; i < transactionsToRemove; i++)
        {
            transactions.pop_front();
        }
    }

    // Add it
    transactions.push_back(newTransaction);

    // If the button was disabled before, we can now enable it
    CMain::get()->getGameWindow()->setUndoButtonEnabled(true);
}

void CGame::undoLastMove()
{

    //TODO: UNDO THIS
    this->isWon = true;
    // Make sure there is at least one transaction to undo
    if (transactions.empty()) return;

    // Last transaction
    Transaction lastTrans = transactions.back();

    qDebug() << "Undoing" << lastTrans.toString();

    // If it was a Stack<->Stack transaction
    if (lastTrans.type == Transaction::TransactionType::StackToStack)
    {
        // If we have to, flip the current top card of the stack1
        if (lastTrans.flipCardBefore && lastTrans.stack1->getTopCard() != nullptr)
        {
            lastTrans.stack1->getTopCard()->setCardFlipped(false);
        }

        // Move the cards back from stack2 to stack1
        for (CCard* card : lastTrans.cards)
        {
            lastTrans.stack2->removeCard(card);
            lastTrans.stack1->addCard(card);
        }
    }
    // If in this transaction the player drew a card from the draw stack
    else if (lastTrans.type == Transaction::TransactionType::DrawFromDrawStack)
    {     
        // Stack1 has to be a draw stack, so call undo without any card
        static_cast<CDrawStack*>(lastTrans.stack1)->undo();
    }
    // If in this transaction the player dragged a card from the draw stack to any other stack
    else if (lastTrans.type == Transaction::TransactionType::DrawToStack)
    {
        // Remove the card from Stack2
        lastTrans.stack2->removeCard(lastTrans.cards[0]);

        // Undo the draw of this card from Stack1 which has to be a draw stack
        static_cast<CDrawStack*>(lastTrans.stack1)->undo(lastTrans.cards[0]);
    }

    // Pop it from the list
    transactions.pop_back();

    // Decrement the moves
    CMain::get()->getGameWindow()->decrementMove();

    // Change the score back
    this->score = lastTrans.scoreBefore;
    emit onScoreChanged();

    // If this was the last transaction to undo, disable the button again
    if (transactions.empty())
    {
        CMain::get()->getGameWindow()->setUndoButtonEnabled(false);
    }
}

void CGame::restartGame()
{
    setUp();
    score = 0;
    isWon = false;
    emit onScoreChanged();
}
