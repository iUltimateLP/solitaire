// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include <QWidget>
#include "Card.h"

// Implements a card stack which can hold multiple cards
class CCardStack : public QWidget
{
    Q_OBJECT

public:
    // Constructor
    CCardStack(QWidget *parent = nullptr);

public:
    // Adds a new card to the stack
    virtual void addCard(CCard* cardToAdd);

    // Removes a card from the stack
    virtual void removeCard(CCard* cardToRemove);

    // Removes a card identified by the card index from the stack
    void removeCardAt(int cardIndexToRemove);

    // Returns the number of cards on this stack
    int getNumCards() const;

    // Gets the current topmost card
    CCard* getTopCard();

    // Checks whether a given card can be dropped on this stack
    virtual bool canDropCard(CCard* cardToDrop);

public slots:
    // Internally handles when the cards on this stack changed
    void handleCardsChanged();

signals:
    // When the cards on this stack changed
    void onCardsChanged();


//TODO: this is currently changed to protected, so that the drawStack can access this List -> is there a better solution?
//private:
protected:
    // Holds the cards for this stack
    QList<CCard*> cards;
};
