// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Game.h"
#include <QDebug>

#include <Main.h>
#include <Card.h>
#include <GameWindow.h>

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

  /*  finalStackHeart = new CFinalStack();
    finalStackDiamond = new CFinalStack();
    finalStackClub = new CFinalStack();
    finalStackSpade = new CFinalStack();

    //shuffle carddeck

    for(int i = 0; i<8; ++i)
    {
        //how to solve holdingStack + i
        holdingStack + i = new CHoldingStack();
        for(int j = 0; j<i; ++j)
        {
            //(holdingStack + i).addCard(carddeck.pop());
        }
    }
    drawPile = new CHoldingStack();
    drawPile.addCards(cardDeck);*/
    //empty carddeck;
    //for detection of ponr: does the game need to know where which card is at the moment?


    //call the CGameWindow to display the initial state of the game
    CCard card(nullptr, ECardSymbol::Heart, ECardType::Ace, 12);
    //how to call this?= with& in param? -> is not working
    CMain::get()->getGameWindow()->displayCards(card);
}

/*
bool CGame::isValidMove(Card from, Card to, CCardStack toStack)
{
    if(toStack.canDropCard(from))
    {
        return true;
    }
    return false;
}

/*
 move-Card checks first if the move is valid
 -> true: adds the card to the destStack and removes the card from the srcStack
    + counts score up
 -> false: shows an error message to the player
*/
/*
CGame::moveCard(Card fromCard, CHoldingStack fromStack, CCardStack toStack)
{
    if(isValidMove(fromCard, toCard))
    {
        fromStack.popCard(fromCard);
        fromStack.flipCard(); //this happens probably instantly in CCardStack
        toStack.addCard(fromCard);
        ++score;
        //where happens the updating of the board? -> CCardStack
    }
    else
    {
        //Message to player: GameWindow.showMessageBox
        //Main.getGameWindow.showMessage(INVALID_MOVE);
    }
}
*/

//game has ended if all finalStacks have 13 cards
bool CGame::hasEnded(){
 /*   if(finalStackClub.getCards().size() == 13 && finalStackDiamond.getCards().size() == 13
            && finalStackHeart.getCards().size() == 13 && finalStackSpade.getCards().size() == 13)
    {
        return true;
    }*/
    return false;
}
