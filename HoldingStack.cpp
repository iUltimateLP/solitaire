// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "HoldingStack.h"
#include <QDebug>
#include <QImage>
#include <QMimeData>
#include <Main.h>

// Set the static variables
int CHoldingStack::CardOffsetInStack = 45;
int CHoldingStack::CardOffsetInStackSmaller = 24;
int CHoldingStack::StackCollapseNumCards = 12;

CHoldingStack::CHoldingStack(QWidget* parent)
    : CCardStack(parent)
{
   // qDebug() << "Created CCardStack";

    // Create the vbox layout
    vbox = new CCardVBoxLayout(CardOffsetInStack, this);
}

void CHoldingStack::addCard(CCard *cardToAdd)
{
    // Add the card to our vbox
    vbox->addWidget(cardToAdd);

    // Call the superclasses' addCard
    CCardStack::addCard(cardToAdd);

    // Check whether we should collapse the stack
    this->checkCollapseStack();
}

void CHoldingStack::removeCard(CCard *cardToRemove)
{
    // Call the superclasses' removeCard
    CCardStack::removeCard(cardToRemove);

    // Remove the card from our vbox
    vbox->removeWidget(cardToRemove);

    // A card was removed from this stack, so flip the next card if the next card is not flipped yet
    // The condition is necessary, otherwise already flipped cards would play the anim when clicking
    // on multiple cards. Also make sure there IS a top card
    if(getTopCard() && !getTopCard()->getFlipped())
    {
        flipNextCard();
    }

    // Check whether we should collapse the stack
    this->checkCollapseStack();
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

        // Also, don't allow dropping kings on aces in holding stacks
        if (topCard->getType() == ECardType::Ace && cardToDrop->getType() == ECardType::King)
        {
            // No aces on kings
            qDebug() << "No aces on kings";
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
    getTopCard()->requestCardFlip(true);

    // Emit that the stack changed
    emit onCardsChanged();
}

QList<CCard*> CHoldingStack::getCardsAbove(CCard* card)
{
    // Will contain the cards
    QList<CCard*> cardsList;

    // Get the index of this card
    int index = getCards().indexOf(card);

    // Make sure the index is valid (= card is in the stack)
    if (index < 0) {
        qDebug() << "Cannot find " << card->toString();
        return cardsList;
    }

    // Iterate over all cards in the region index..numCards
    for (int i = index; i < getNumCards(); i++)
    {
        // Add it to the list
        cardsList.push_back(getCards()[i]);
    }

    return cardsList;
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
    // Deserialize the payload from the MIME data
    QByteArray mimeData = ev->mimeData()->data("application/x-solitaire-dnd");
    QDataStream mimeDataStream(&mimeData, QIODevice::ReadOnly);
    CardDndPayload* payload;
    mimeDataStream >> payload;

    // Make sure the payload is valid
    if (!payload) return;

    // Make sure there is a card
    if (payload->cards.length() <= 0) return;

    // Now, this drop event will only be called if we can actually drop here, the actual
    // decision happens in dragEnterEvent

    // Call the method to change the score in the GameInstance with the suitable enum
    if (dynamic_cast<CFinalStack*>(payload->cards[0]->getCardStack()) != NULL)
    {
        // If the card comes from a FinalStack
        CMain::get()->getGameInstance()->changeScore(GameScoringAttributes::FOUNDATION_TO_TABLEAU);
    }
    else if(dynamic_cast<CDrawStack*>(payload->cards[0]->getCardStack()) != NULL)
    {
        // If the card comes from the DrawStack
        CMain::get()->getGameInstance()->changeScore(GameScoringAttributes::WASTE_PILE_TO_TABLEAU);
    }
    else if(dynamic_cast<CFinalStack*>(payload->cards[0]->getCardStack()) != NULL)
    {
        // If the card comes from the DrawStack
        CMain::get()->getGameInstance()->changeScore(GameScoringAttributes::FOUNDATION_TO_TABLEAU);
    }

    // Register a new transaction
    Transaction t;
    t.type = Transaction::TransactionType::StackToStack;
    t.stack1 = payload->cards.front()->getCardStack();
    t.stack2 = this;
    t.cards = payload->cards;
    CMain::get()->getGameInstance()->addTransaction(t);

    // Go through all cards we're about to drop
    for (CCard* card : payload->cards)
    {
        // Remove the card from the stack it's in right now, if any
        if (card->getCardStack())
        {
            card->getCardStack()->removeCard(card);
        }

        // Add it to this stack
        this->addCard(card);
    }

    // Increment the amount of steps
    CMain::get()->getGameWindow()->incrementMove();

    // Play a sound
    CMain::get()->getSoundManager()->playSoundEffect(SoundEffectType::CardClick);
}

void CHoldingStack::checkCollapseStack()
{
    // Check whether we should collapse
    if (this->getNumCards() >= CHoldingStack::StackCollapseNumCards)
    {
        // Set the spacing to smaller
        vbox->setSpacingWithAnimation(CHoldingStack::CardOffsetInStackSmaller);
    }
    else
    {
        // Set the spacing back to normla
        vbox->setSpacingWithAnimation(CHoldingStack::CardOffsetInStack);
    }
}
