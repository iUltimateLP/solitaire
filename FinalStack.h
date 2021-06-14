// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include "CardStack.h"

// Represents one of the four final card stacks at the top of the game,
// where cards get dropped off until the stack is full
class CFinalStack : public CCardStack
{
    Q_OBJECT

public:
    // Constructor
    CFinalStack(QWidget* parent = nullptr, ECardSymbol symbol = ECardSymbol::Heart);

public:
    //~ Begin CCardStack interface
    virtual void addCard(CCard *cardToAdd) override;
    virtual void removeCard(CCard *cardToRemove) override;
    virtual bool canDropCard(CCard* cardToDrop) override;
    //~ End CCardCard interface

private:
    // The symbol this final stack allows
    ECardSymbol stackSymbol;
};

