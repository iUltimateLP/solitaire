// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once
#include <QObject>
#include "CardStack.h"
#include "CardVBoxLayout.h"


class CDrawStack : public CCardStack
{
    Q_OBJECT

public:
    CDrawStack(QWidget* parent = nullptr);

public:
    //~ Begin CCardStack interface
    virtual void addCard(CCard *cardToAdd) override;
    virtual void removeCard(CCard *cardToRemove) override;
    virtual bool canDropCard(CCard* cardToDrop) override;
    //~ End CCardCard interface

    // This function is called in the beginning to set the first card visible and the variable currentCard to the top one
    void setUp();

public slots:
    void showNextCard();

private:
    // Box layout to display the cards
    CCardVBoxLayout* vbox;

    // Holds the index of the current visible card
    int currentCard = 0;
};


