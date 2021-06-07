// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "CardStack.h"
#include <QDebug>

CCardStack::CCardStack(QWidget *parent)
    : QWidget(parent)
{
    // Connect the event
    QObject::connect(this, &CCardStack::onCardsChanged, this, &CCardStack::handleCardsChanged);
}

void CCardStack::addCard(CCard *cardToAdd)
{
    qDebug() << "CCardStack::addCard";

    // Make sure the card is valid
    if (cardToAdd)
    {
        // Add it to the list
        cards.push_back(cardToAdd);

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

        // Notify the handler
        emit onCardsChanged();
    }
}

void CCardStack::removeCard(int cardIndexToRemove)
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

bool CCardStack::canDropCard(CCard *cardToDrop)
{
    qDebug() << "CanDropCard not overwritten!";

    // By default, allow dropping
    return true;
}

void CCardStack::handleCardsChanged()
{
    qDebug() << "handleCardsChanged!";
}
