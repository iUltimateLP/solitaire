// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "FinalStack.h"
#include "CardVBoxLayout.h"
#include <QDebug>

CFinalStack::CFinalStack(QWidget* parent, ECardSymbol symbol)
    : CCardStack(parent)
    , stackSymbol(symbol)
{
    // Cut out the placeholder image from the tileset
    int tilesetX = (int)symbol;
    QPixmap placeholderTilset = QPixmap(":/assets/card_placeholders.png");
    QPixmap placeholderPixmap = placeholderTilset.copy(tilesetX * CCard::cardTileSize.width(), 0, CCard::cardTileSize.width(), CCard::cardTileSize.height());

    // Create a new placeholder label with that image
    QLabel* placeholder = new QLabel(this);
    placeholder->setGeometry(0, 0, CCard::getCardScreenSize().width(), CCard::getCardScreenSize().height());
    placeholder->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    placeholder->setPixmap(placeholderPixmap.scaled(CCard::getCardScreenSize().width(), CCard::getCardScreenSize().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // This widget has always the size of a card
    this->setGeometry(0, 0, CCard::cardTileSize.width(), CCard::cardTileSize.height());

    // Create the dummy vbox layout with zero spacing, so they get overlayed
    vbox = new CCardVBoxLayout(0, this);
}

void CFinalStack::addCard(CCard* cardToAdd)
{
    // Hide the current top card, if any
    /*if (this->getNumCards() > 0)
    {
        this->getTopCard()->hide();
    }*/

    // Call the superclasses' addCard
    CCardStack::addCard(cardToAdd);

    vbox->addWidget(cardToAdd);
}

void CFinalStack::removeCard(CCard* cardToRemove)
{
    // We can only remove the top card
    if (cardToRemove != this->getTopCard()) return;

    // Call the superclasses' removeCard
    CCardStack::removeCard(cardToRemove);

    vbox->removeWidget(cardToRemove);

    // Show the next top card, if any
    /*if (this->getNumCards() > 0)
    {
        this->getTopCard()->show();
    }*/
}

bool CFinalStack::canDropCard(CCard *cardToDrop)
{
    // Check if the symbol is what we allow on this stack
    if (cardToDrop->getSymbol() != stackSymbol) return false;

    // Get the top card on the stack
    CCard* topCard = this->getTopCard();

    // If we have any card
    if (topCard)
    {
        // If the top card is an Ace, the next allowed card is only a '2'
        if (topCard->getType() == ECardType::Ace && cardToDrop->getNumberValue() == 2) return true;

        // Check whether the value of the card to drop is one higher than the current top card
        if (cardToDrop->getOverallValue() != topCard->getOverallValue() + 1) return false;
    }
    else
    {
        // When no card is on this stack, the first card to accept is only an Ace
        if (cardToDrop->getType() != ECardType::Ace) return false;
    }

    // Allowed
    return true;
}

void CFinalStack::dragEnterEvent(QDragEnterEvent* ev)
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

void CFinalStack::dropEvent(QDropEvent* ev)
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
