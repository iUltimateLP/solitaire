// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "FinalStack.h"

CFinalStack::CFinalStack(QWidget* parent, ECardSymbol symbol)
    : CCardStack(parent)
    , stackSymbol(symbol)
{
    QLabel lbl("abc", parent);
    lbl.resize(CCard::getCardScreenSize().width(), CCard::getCardScreenSize().height());

    // This widget has always the size of a card
    this->resize(CCard::getCardScreenSize().width(), CCard::getCardScreenSize().height());
    this->setGeometry(0,0,170,255);

    this->setStyleSheet("background-color: rgba(255, 255, 255, 1);");
}

void CFinalStack::addCard(CCard *cardToAdd)
{
    // Hide the current top card, if any
    if (this->getNumCards() > 0)
    {
        this->getTopCard()->hide();
    }

    // Call the superclasses' addCard
    CCardStack::addCard(cardToAdd);
}

void CFinalStack::removeCard(CCard *cardToRemove)
{
    // We can only remove the top card
    if (cardToRemove != this->getTopCard()) return;

    // Call the superclasses' removeCard
    CCardStack::removeCard(cardToRemove);

    // Show the next top card, if any
    if (this->getNumCards() > 0)
    {
        this->getTopCard()->show();
    }
}

bool CFinalStack::canDropCard(CCard *cardToDrop)
{
    // Check if the symbol is what we allow on this stack
    if (cardToDrop->getSymbol() != stackSymbol) return false;

    // If we have any card
    if (this->getNumCards() > 0)
    {
        // Get the top card on the stack
        CCard* topCard = this->getTopCard();

        // Check whether the value of the card to drop is one higher than the current top card
        if (cardToDrop->getOverallValue() != topCard->getOverallValue() + 1) return false;
    }

    // Allowed
    return true;
}
