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
            deck.push_back(new CCard(nullptr, s, ECardType::Number, i));
        }
        for(ECardType t: typeVector)
        {
            deck.push_back(new CCard(nullptr, s, t, 0));
        }
    }

    for(int i = 0; i < deck.length(); ++i)
    {
        qDebug() << deck[i]->getSymbol();
        qDebug() << deck[i]->getType();
        qDebug() << deck[i]->getNumberValue();
    }

    //declaring the holding Stacks
    hold1 = new CHoldingStack();
    hold2 = new CHoldingStack();
    hold3 = new CHoldingStack();
    hold4 = new CHoldingStack();
    hold5 = new CHoldingStack();
    hold6 = new CHoldingStack();
    hold7 = new CHoldingStack();

    //shuffle carddeck
    std::random_shuffle(deck.begin(), deck.end());


    //TODO: write a loop for this!
    //filling the cardStacks with cards from the deck
    hold1->addCard(deck.front());
    deck.pop_front();
    hold2->addCard(deck.front());
    deck.pop_front();
    hold2->addCard(deck.front());
    deck.pop_front();
    hold3->addCard(deck.front());
        deck.pop_front();
    hold3->addCard(deck.front());
        deck.pop_front();
    hold3->addCard(deck.front());
        deck.pop_front();
    hold4->addCard(deck.front());
    deck.pop_front();
    hold4->addCard(deck.front());
        deck.pop_front();
    hold4->addCard(deck.front());
        deck.pop_front();
    hold4->addCard(deck.front());
        deck.pop_front();
    hold5->addCard(deck.front());
        deck.pop_front();
    hold5->addCard(deck.front());
        deck.pop_front();
    hold5->addCard(deck.front());
        deck.pop_front();
    hold5->addCard(deck.front());
        deck.pop_front();
    hold5->addCard(deck.front());
        deck.pop_front();
    hold6->addCard(deck.front());
        deck.pop_front();
    hold6->addCard(deck.front());
        deck.pop_front();
    hold6->addCard(deck.front());
        deck.pop_front();
    hold6->addCard(deck.front());
        deck.pop_front();
    hold6->addCard(deck.front());
        deck.pop_front();
    hold6->addCard(deck.front());
      deck.pop_front();
    hold7->addCard(deck.front());
        deck.pop_front();
    hold7->addCard(deck.front());
       deck.pop_front();
    hold7->addCard(deck.front());
        deck.pop_front();
    hold7->addCard(deck.front());
        deck.pop_front();
    hold7->addCard(deck.front());
       deck.pop_front();
    hold7->addCard(deck.front());
        deck.pop_front();
    hold7->addCard(deck.front());

    /*
    for(int i = 0; i<7; ++i)
    {
        for(int j = 0; j<i; ++j)
        {
            //(holdingStack + i).addCard(carddeck.pop());
        }
    }
    */
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
    CMain::get()->getGameWindow()->displayHoldingStack(hold1);
    CMain::get()->getGameWindow()->displayHoldingStack(hold2);
    CMain::get()->getGameWindow()->displayHoldingStack(hold3);
    CMain::get()->getGameWindow()->displayHoldingStack(hold4);
    CMain::get()->getGameWindow()->displayHoldingStack(hold5);
    CMain::get()->getGameWindow()->displayHoldingStack(hold6);
    CMain::get()->getGameWindow()->displayHoldingStack(hold7);

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
         //   srcStack->removeCard(srcStack->getNumCards()-1);
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
