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
    setUp();
}

//the initial setup of the game board:
//shuffles carddeck, fills all cardstacks and calls the gamewindow to display everything
void CGame::setUp()
{
    const ECardSymbol symbolVector[] = {Club, Heart, Diamond, Spade};
    const ECardType typeVector[] = {Jack, Queen, King, Ace};

    //filling the deck initially
    //TODO: which type instead of int?
    for(ECardSymbol s: symbolVector)
    {
        for (int i = 2; i < 11; ++i)
        {
            CCard* newCard = new CCard(nullptr, s, ECardType::Number, i);
            deck.push_back(newCard);
        }
        for(ECardType t: typeVector)
        {
            CCard* newCard = new CCard(nullptr, s, t, 0);
            deck.push_back(newCard);
        }
    }

    //shuffle carddeck
    std::random_shuffle(deck.begin(), deck.end());

    //declaring the holding Stacks in a list


    for(int i = 0; i<7; ++i)
    {
        holdingStacks.push_back(new CHoldingStack());

        for(int j = 0; j<i+1; ++j)
        {
            holdingStacks[i]->addCard(deck.front());
            deck.pop_front();
        }
    }

  //  drawPile = new CCardStack();
 //   drawPile.addCards(cardDeck);*/
    //empty carddeck;
    //for detection of ponr: does the game need to know where which card is at the moment?

    //placeholder for single symbols -> use FinalCardStack or single Cards in the end
    finalHeart = new CHoldingStack();
    finalHeart->addCard(new CCard(nullptr, ECardSymbol::Heart, ECardType::Ace, 12));
    finalDiamond = new CHoldingStack();
    finalDiamond->addCard(new CCard(nullptr, ECardSymbol::Diamond, ECardType::Ace, 12));
    finalClub = new CHoldingStack();
    finalClub->addCard(new CCard(nullptr, ECardSymbol::Club, ECardType::Ace, 12));
    finalSpade = new CHoldingStack();
    finalSpade->addCard(new CCard(nullptr, ECardSymbol::Spade, ECardType::Ace, 12));

    //call the CGameWindow to display the initial state of the game
    for(CHoldingStack* stack: holdingStacks)
    {
        CMain::get()->getGameWindow()->displayHoldingStack(stack);
    }

    CMain::get()->getGameWindow()->displayFinalStack(finalHeart);
    CMain::get()->getGameWindow()->displayFinalStack(finalDiamond);
    CMain::get()->getGameWindow()->displayFinalStack(finalSpade);
    CMain::get()->getGameWindow()->displayFinalStack(finalClub);
}

/*
 move-Card checks first if the move is valid
 -> true: adds the card to the destStack and removes the card from the srcStack
    + counts score up
 -> false: shows an error message to the player
*/
void CGame::moveCard(CCard* cardToDrop, CHoldingStack* srcStack, CCardStack* destStack)
{
    if(destStack->canDropCard(cardToDrop))
    {
        QList<CCard*> cardsToMove;
        //while cardToDrop is not the last card, all cards are safed in list and removed
        while (cardToDrop != srcStack->getTopCard())
        {
            cardsToMove.push_back(srcStack->getTopCard());
            //TODO: this function call doesn't work -> does not detect overloading
            //srcStack->removeCardAt(srcStack->getNumCards()-1);
        }
        //finally remove the cardToDrop from the source stack and add it to the destination stack
        srcStack->removeCard(cardToDrop);
        destStack->addCard(cardToDrop);
        //while cardsToMove list is not empty, add the cards to destination stack
        while(cardsToMove.size() > 0)
        {
              destStack->addCard(cardsToMove.front());
              cardsToMove.pop_front();
        }
        CMain::get()->getGameWindow()->incrementScore();
    }
    else
    {
        //Message to player: GameWindow.showMessageBox
        //Main.getGameWindow.showMessage(INVALID_MOVE);
    }
}

//game has ended if all finalStacks have 13 cards
bool CGame::hasEnded(){
    if(finalDiamond->getNumCards() == 13 && finalSpade->getNumCards() == 13
            && finalHeart->getNumCards() == 13 && finalSpade->getNumCards() == 13)
    {
        return true;
    }
    return false;
}
