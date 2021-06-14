// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "HoldingStack.h"
#include <QDebug>
#include <QImage>

CHoldingStack::CHoldingStack(QWidget* parent)
    : CCardStack(parent)
{
    qDebug() << "Created CCardStack";

    // Create the vbox layout
    vbox = new CCardVBoxLayout(45, this);
}

void CHoldingStack::addCard(CCard *cardToAdd)
{
    // Call the superclasses' addCard
    CCardStack::addCard(cardToAdd);

    // Add the card to our vbox
    vbox->addWidget(cardToAdd);
}

void CHoldingStack::removeCard(CCard *cardToRemove)
{
    // Call the superclasses' removeCard
    CCardStack::removeCard(cardToRemove);

    // Remove the card from our vbox
    vbox->removeWidget(cardToRemove);
}

bool CHoldingStack::canDropCard(CCard *cardToDrop)
{
    // If we have any card
    if (this->getNumCards() > 0)
    {
        // Get the top card on the stack
        CCard* topCard = this->getTopCard();

        // First, check if it's the opposite color
        bool isTopCardRed = topCard->isRed();
        bool isCardToDropRed = cardToDrop->isRed();

        if ((isTopCardRed && isCardToDropRed) || (!isTopCardRed && !isCardToDropRed))
        {
            // No same colors
            qDebug() << "No same color";
            return false;
        }

        // Now, check if the value of this card is one lower than the current top card
        if (cardToDrop->getOverallValue() != topCard->getOverallValue() - 1)
        {
            // No lower value
            qDebug() << "No lower value!";
            return false;
        }

        // If all of these conditions passed, we allow it
        return true;
    }

    // If we don't have any card on this stack, we can always drop
    return true;
}
