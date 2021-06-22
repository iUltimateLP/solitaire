// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "HoldingStack.h"
#include <QDebug>
#include <QImage>
#include <QMimeData>

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

    // A card was removed from this stack, so flip the next card
    flipNextCard();
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
    else
    {
        // No card, then only a king can start a new stack
        if (cardToDrop->getType() != ECardType::King) return false;
    }

    // If we don't have any card on this stack, we can always drop
    return true;
}

void CHoldingStack::flipNextCard()
{
    // Make sure we have any card
    if (getNumCards() == 0) return;

    // Flip the top card
    getTopCard()->setCardFlipped(true);

    // Emit that the stack changed
    emit onCardsChanged();
}

void CHoldingStack::dragEnterEvent(QDragEnterEvent *ev)
{
    // We can extract the card to drop from the source of the drag event (as we started
    // the d'n'd operation in the card)
    CCard* cardToDrop = reinterpret_cast<CCard*>(ev->source());

    // Make sure the card is valid
    if (!cardToDrop) return;

    // Check if we could drop the card here
    if (this->canDropCard(cardToDrop))
    {
        // Accept the drag operation
        ev->acceptProposedAction();
    }
}

void CHoldingStack::dropEvent(QDropEvent* ev)
{
    // We can extract the card to drop from the source of the drag event (as we started
    // the d'n'd operation in the card)
    CCard* cardToDrop = reinterpret_cast<CCard*>(ev->source());

    // Make sure the card is valid
    if (!cardToDrop) return;

    // Now, this drop event will only be called if we can actually drop here, the actual
    // decision happens in dragEnterEvent

    // Remove the card from the stack it's in right now, if it's in any stack
    if (cardToDrop->getCardStack())
    {
        cardToDrop->getCardStack()->removeCard(cardToDrop);
    }

    // Add it to this stack
    this->addCard(cardToDrop);
}
