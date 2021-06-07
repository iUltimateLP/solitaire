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
    virtual void addCard(CCard *cardToAdd) override;

    virtual void removeCard(CCard *cardToRemove) override;

    // Checks whether a given card can be dropped on this stack
    virtual bool canDropCard(CCard* cardToDrop) override;

private:
    // The vertical box layout to use for the holding stack
    CCardVBoxLayout* vbox;
};
