// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "DrawStack.h"
#include "CardVBoxLayout.h"
#include <QDebug>

#include <Main.h>

CDrawStack::CDrawStack(QWidget* parent)
    : CCardStack(parent)
{
    // This widget has always the size of a card
    this->setGeometry(0, 0, CCard::cardTileSize.width(), CCard::cardTileSize.height());

    // Create the dummy vbox layout with zero spacing, so they get overlayed
    vbox = new CCardVBoxLayout(0, this);
}

void CDrawStack::addCard(CCard* cardToAdd)
{
    CCardStack::addCard(cardToAdd);

    vbox->addWidget(cardToAdd);
}

void CDrawStack::removeCard(CCard* cardToRemove)
{
    // Call the superclasses' removeCard
    CCardStack::removeCard(cardToRemove);

    // When a card is removed, the next card in the stack will be displayed
    --currentCard;
    if(currentCard > 0)
    {
       this->cards[currentCard]->setVisible(1);
    }

    vbox->removeWidget(cardToRemove);

}

bool CDrawStack::canDropCard(CCard *cardToDrop)
{
    // Adding a card to the drawStack is not possible, due to that, this method is not necessary
    qDebug() << "nothing happens";
}

void CDrawStack::showNextCard()
{
    // The current card is set to invisible
    this->cards[currentCard]->setVisible(0);
    // If there are cards left on the stack, the variable "currentCard" gets decremented
    if(currentCard > 0 )
    {
        --currentCard;
        if(currentCard > 0 )
        {
            // If there are still cards after decrementing the variable, the following card is set to visible
            this->cards[currentCard]->setVisible(1);
        }
        else
        {
            CMain::get()->getGameWindow()->toggleDrawStackPlaceholder();
        }
    }
    // Otherwise, the stack is recycled by setting the currentCard to the topCard again
    else
    {
        currentCard = getNumCards()-1;
        this->cards[currentCard]->setVisible(1);
    }
    qDebug() << "click and show next card";
}

void CDrawStack::setUp()
{
    getTopCard()->setVisible(1);
    this->currentCard = getNumCards()-1;
}
