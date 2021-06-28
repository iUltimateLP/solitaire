// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Game.h"
#include <QDebug>

#include <Main.h>

#include "GameWindow.h"
#include "HoldingStack.h"
#include "CardStack.h"
#include "FinalStack.h"

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

    // REMOVE THESE WHEN IMPLEMENTED SCORING
    /*  Q_UNUSED(GameScoringAttributes::FOUNDATION_TO_TABLEAU);
    Q_UNUSED(GameScoringAttributes::TURN_OVER_TABLEAU_CARD);
    Q_UNUSED(GameScoringAttributes::TABLEAU_TO_FOUNDATION);
    Q_UNUSED(GameScoringAttributes::WASTE_PILE_TO_FOUNDATION);
    Q_UNUSED(GameScoringAttributes::WASTE_PILE_TO_TABLEAU);
    Q_UNUSED(GameScoringAttributes::RECYCLING_DRAW_PILE);*/
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

    // Declaration of the 4 final stacks, these are initially empty
    finalStacks.append(new CFinalStack(CMain::get()->getGameWindow(), ECardSymbol::Heart));
    finalStacks.append(new CFinalStack(CMain::get()->getGameWindow(), ECardSymbol::Diamond));
    finalStacks.append(new CFinalStack(CMain::get()->getGameWindow(), ECardSymbol::Club));
    finalStacks.append(new CFinalStack(CMain::get()->getGameWindow(), ECardSymbol::Spade));

    // Call the CGameWindow to display the initial state of the finalStacks
    for(int i = 0; i < 4; ++i)
    {
        CMain::get()->getGameWindow()->displayFinalStack(finalStacks[i], i);
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
    for(int i = 0; i < 4; i++)
   // for(int i = 0; i < deck.size(); ++i)
    {
        drawStack->addCard(deck[i]);
    }

    // Call the CGameWindow to display the initial state of the drawStack and clear the deck
    CMain::get()->getGameWindow()->displayDrawStack(drawStack);
    deck.clear();
}

void CGame::moveCard(CCard* cardToDrop, CCardStack* srcStack)
{
    // Iterate over the stacks and check if card can be dropped
    for(CFinalStack* final: finalStacks)
    {
        // If the card fits on a final stack, it has to be the top card as only a single card can be dropped there
        if(final->canDropCard(cardToDrop) && cardToDrop == srcStack->getTopCard())
        {
            srcStack->removeCard(cardToDrop);
            final->addCard(cardToDrop);

            // Increment the amount of steps
            CMain::get()->getGameWindow()->incrementMove();
            return;
        }
    }
    for(CHoldingStack* holding: holdingStacks)
    {
        if(holding->canDropCard(cardToDrop))
        {
            // We need a list, if the cardToDrop is not the topCard
            QList<CCard*> cardsToMove;

            // While cardToDrop is not the last card, all cards are safed in list and removed
            while (cardToDrop != srcStack->getTopCard())
            {
                cardsToMove.push_back(srcStack->getTopCard());
                srcStack->removeCardAt(srcStack->getNumCards()-1);
            }

            // Finally remove the cardToDrop from the source stack and add it to the destination stack
            srcStack->removeCard(cardToDrop);
            holding->addCard(cardToDrop);

            // While cardsToMove list is not empty, add the cards to destination stack
            while(cardsToMove.size() > 0)
            {
                holding->addCard(cardsToMove.front());
                cardsToMove.pop_front();
            }

            // Increment the amount of steps
            CMain::get()->getGameWindow()->incrementMove();
            return;
        }

        // TODO: Scoring
    }
    /*
        // Check if the move has an impact on the score -> this is done before the movement, so it can be checked if a card will be flipped
        evaluateScore(srcStack, destStack);
    }*/
}

void CGame::evaluateScore(CCardStack *srcStack, CCardStack *dstStack)
{
    // Remove if used
    Q_UNUSED(srcStack);
    Q_UNUSED(dstStack);

    /*
    // Increment the score with the suitable attribute
    if(dynamic_cast<CHoldingStack*>(srcStack) != NULL && dynamic_cast<CFinalStack*>(dstStack) != NULL)
    {
        // Points for moving a card from a HoldingStack to a FinalStack
        changeScore(GameScoringAttributes::TABLEAU_TO_FOUNDATION);
        if (srcStack.getNumberUnflippedCards() > 0)
        {
            // Extra points if the next card from the HoldingStack gets flipped
            changeScore(GameScoringAttributes::TURN_OVER_TABLEAU_CARD);
        }
    }
    else if (dynamic_cast<CFinalStack*>(srcStack) != NULL && dynamic_cast<CHoldingStack*>(dstStack) != NULL)
    {
        // Minus points if card is moved from FinalStack back to a HoldingStack
        changeScore(GameScoringAttributes::FOUNDATION_TO_TABLEAU);
    }
    else if(dynamic_cast<CHoldingStack*>(srcStack) != NULL && dynamic_cast<CHoldingStack*>(dstStack) != NULL)
    {
        if(srcStack->getNumberUnflippedCards() > 0)
        {
            // Extra points if the next card from the HoldingStack gets flipped
            changeScore(GameScoringAttributes::TURN_OVER_TABLEAU_CARD);
        }
    }
    else if (dynamic_cast<CDrawStack*>(srcStack) != NULL && dynamic_cast<CHoldingStack*>(dstStack) != NULL)
    {
        //TODO: check if DrawStack is empty -> "incrementScore" and recycle
        if (srcStack.isEmpty())
        {
            srcStack->recycle();
            // Minus points if the DrawStack is empty and gets recycled
            changeScore(GameScoringAttributes::RECYCLING_DRAW_PILE);
        }
        // Points for moving a card from the DrawStack to a HoldingStack
        changeScore(GameScoringAttributes::WASTE_PILE_TO_TABLEAU);
    }
    else if (dynamic_cast<CDrawStack*>(srcStack) != NULL && dynamic_cast<CFinalStack*>(dstStack) != NULL)
    {
        //TODO: check if DrawStack is empty -> "incrementScore" and recycle
        // Points for moving a card directly from the DrawStack to a FinalStack
        changeScore(GameScoringAttributes::WASTE_PILE_TO_FOUNDATION);
    }
    */
}

void CGame::changeScore(int points)
{
    score += points;
    if(score < 0)
    {
        score = 0;
    }
    emit onScoreChanged();
}

int CGame::getScore()
{
    return score;
}

bool CGame::hasEnded()
{
    // Check if all final stacks got 13 cards
    // TODO: use finalXY->isComplete() when implemented
    return (finalDiamond->getNumCards() == 13
            && finalSpade->getNumCards() == 13
            && finalHeart->getNumCards() == 13
            && finalSpade->getNumCards() == 13);
}

void CGame::restartGame()
{
    setUp();
    score = 0;
    emit onScoreChanged();
}
