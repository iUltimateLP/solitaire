// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "CardStack.h"
#include <QDebug>

CCardStack::CCardStack(QWidget *parent)
    : QWidget(parent)
{
    // Connect the event
    QObject::connect(this, &CCardStack::onCardsChanged, this, &CCardStack::handleCardsChanged);

    // Enable dropping elements
    setAcceptDrops(true);
}

void CCardStack::addCard(CCard *cardToAdd)
{
    // Make sure the card is valid
    if (cardToAdd)
    {
        // Add it to the list
        cards.push_back(cardToAdd);

        // Set the card's stack to this one
        cardToAdd->setCardStack(this);

        // Notify the handler
        emit onCardsChanged();
    }
}

void CCardStack::insertCardAt(int insertAt, CCard* cardToInsert)
{
    // Make sure the card is valid
    if (cardToInsert)
    {
        // Add it to the list
        cards.insert(insertAt, cardToInsert);

        // Set the card's stack to this one
        cardToInsert->setCardStack(this);

        // Notify the handler
        emit onCardsChanged();
    }
}

void CCardStack::removeCard(CCard *cardToRemove)
{
    qDebug() << "CCardStack::removeCard";

    // Make sure the card is valid
    if (cardToRemove)
    {
        // Remove it from the list
        cards.removeOne(cardToRemove);

        // Clear the card's stack
        cardToRemove->setCardStack(nullptr);

        // Notify the handler
        emit onCardsChanged();
    }
}

void CCardStack::removeCardAt(int cardIndexToRemove)
{
    // Make sure the index is valid
    if (cardIndexToRemove >= 0 && cardIndexToRemove < cards.length())
    {
        // Remove it from the list
        removeCard(cards[cardIndexToRemove]);
    }
}

int CCardStack::getNumCards() const
{
    // Return the length of the cards list
    return cards.length();
}

CCard* CCardStack::getTopCard()
{
    // Make sure we have cards
    if (cards.length() > 0)
    {
        // Return the last card in the list, as that will be the topmost card
        return cards.last();
    }
    else
    {
        // Return nothing
        return nullptr;
    }
}

bool CCardStack::canDropCard(CCard* cardToDrop)
{
    Q_UNUSED(cardToDrop);

    // By default, allow dropping
    return true;
}

int CCardStack::getIndexOfCard(CCard* card)
{
    // Find the card
    return cards.indexOf(card);
}

void CCardStack::handleCardsChanged()
{

}
