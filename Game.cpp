// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Game.h"
#include <QDebug>

#include <Main.h>

#include <GameWindow.h>
#include <HoldingStack.h>
#include <CardStack.h>

CGame::CGame(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Created CGame";

    // Initially set up the game
    setUp();
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
            CCard* newCard = new CCard(nullptr, symbol, ECardType::Number, i);
            deck.push_back(newCard);
        }

        // Create all special cards
        for(ECardType type : typeVector)
        {
            CCard* newCard = new CCard(nullptr, symbol, type, 0);
            deck.push_back(newCard);
        }
    }

    // Shuffle carddeck
    std::random_shuffle(deck.begin(), deck.end());

    // Create seven holding stacks and fill with cards
    for(int i = 0; i < 7; ++i)
    {
        // Add a new holding stack
        holdingStacks.push_back(new CHoldingStack());

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

    // drawPile = new CCardStack();
    // drawPile.addCards(cardDeck);
    // For detection of ponr: does the game need to know where which card is at the moment?

    // Placeholder for single symbols => use FinalCardStack or single Cards in the end
    finalHeart = new CHoldingStack();
    finalHeart->addCard(new CCard(nullptr, ECardSymbol::Heart, ECardType::Ace, 12));
    finalDiamond = new CHoldingStack();
    finalDiamond->addCard(new CCard(nullptr, ECardSymbol::Diamond, ECardType::Ace, 12));
    finalClub = new CHoldingStack();
    finalClub->addCard(new CCard(nullptr, ECardSymbol::Club, ECardType::Ace, 12));
    finalSpade = new CHoldingStack();
    finalSpade->addCard(new CCard(nullptr, ECardSymbol::Spade, ECardType::Ace, 12));

    // Call the CGameWindow to display the initial state of the game
    for(CHoldingStack* stack: holdingStacks)
    {
        CMain::get()->getGameWindow()->displayHoldingStack(stack);
    }

    CMain::get()->getGameWindow()->displayFinalStack(finalHeart);
    CMain::get()->getGameWindow()->displayFinalStack(finalDiamond);
    CMain::get()->getGameWindow()->displayFinalStack(finalSpade);
    CMain::get()->getGameWindow()->displayFinalStack(finalClub);
}

void CGame::moveCard(CCard* cardToDrop, CCardStack* srcStack, CCardStack* destStack)
{
    // Can we drop the card?
    if(destStack->canDropCard(cardToDrop))
    {
/*

        // Increment the score with the suitable attribute (this is done before the movement, so it can be checked if there are unflipped cards on the srcStack)
        if(dynamic_cast<CHoldingStack*>(srcStack) != NULL && dynamic_cast<CFinalStack*>(dstStack) != NULL)
        {
            CMain::get()->getGameWindow()->incrementScore(GameScoringAttributes::TABLEAU_TO_FOUNDATION);
            if (srcStack.getNumberUnflippedCards() > 0)
            {
                CMain::get()->getGameWindow()->incrementScore(GameScoringAttributes::TURN_OVER_TABLEAU_CARD);
            }
        }
        else if (dynamic_cast<CFinalStack*>(srcStack) != NULL && dynamic_cast<CHoldingStack*>(dstStack) != NULL)
        {
            CMain::get()->getGameWindow()->incrementScore(GameScoringAttributes::FOUNDATION_TO_TABLEAU);
        }
        else if (dynamic_cast<CDrawStack*>(srcStack) != NULL && dynamic_cast<CHoldingStack*>(dstStack) != NULL)
        {
            //TODO: check if DrawStack is empty -> "incrementScore" and reset
            /*
              if (srcStack.isEmpty())
              {
                srcStack.reset();
                CMain::get()->getGameWindow()->incrementScore(GameScoringAttributes::RECYCLING_DRAW_PILE);
              }

            CMain::get()->getGameWindow()->incrementScore(GameScoringAttributes::WASTE_PILE_TO_TABLEAU);
        }
        else if (dynamic_cast<CDrawStack*>(srcStack) != NULL && dynamic_cast<CFinalStack*>(dstStack) != NULL)
        {
            //TODO: check if DrawStack is empty -> "incrementScore" and reset
            CMain::get()->getGameWindow()->incrementScore(GameScoringAttributes::WASTE_PILE_TO_FOUNDATION);
        }
*/

        // Movement of the card:
        QList<CCard*> cardsToMove;

        // While cardToDrop is not the last card, all cards are safed in list and removed
        while (cardToDrop != srcStack->getTopCard())
        {
            cardsToMove.push_back(srcStack->getTopCard());
            srcStack->removeCardAt(srcStack->getNumCards()-1);
        }

        // Finally remove the cardToDrop from the source stack and add it to the destination stack
        srcStack->removeCard(cardToDrop);
        destStack->addCard(cardToDrop);

        // While cardsToMove list is not empty, add the cards to destination stack
        while(cardsToMove.size() > 0)
        {
              destStack->addCard(cardsToMove.front());
              cardsToMove.pop_front();
        }

        // Increment the amount of steps
        CMain::get()->getGameWindow()->incrementMove();
    }
}

bool CGame::hasEnded()
{
    // Check if all final stacks got 13 cards
    return (finalDiamond->getNumCards() == 13
            && finalSpade->getNumCards() == 13
            && finalHeart->getNumCards() == 13
            && finalSpade->getNumCards() == 13);
}
