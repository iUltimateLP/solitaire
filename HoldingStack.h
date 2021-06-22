// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include <QWidget>
#include "CardStack.h"
#include "CardVBoxLayout.h"

// The holding stack is a card stack which renders the cards below each other
class CHoldingStack : public CCardStack
{
    Q_OBJECT

public:
    // Constructor
    CHoldingStack(QWidget* parent = nullptr);

public:
    //~ Begin CCardStack interface
    virtual void addCard(CCard *cardToAdd) override;
    virtual void removeCard(CCard *cardToRemove) override;
    virtual bool canDropCard(CCard* cardToDrop) override;
    //~ End CCardCard interface

    // Flips the next top card
    void flipNextCard();

    // Returns all cards above a specific card
    QList<CCard*> getCardsAbove(CCard* card);

private:
    // Overwritten drag'n'drop events
    void dragEnterEvent(QDragEnterEvent* ev) override;
    void dropEvent(QDropEvent* ev) override;

private:
    // The vertical box layout to use for the holding stack
    CCardVBoxLayout* vbox;
};
