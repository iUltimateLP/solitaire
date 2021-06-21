// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once
#include <QObject>
#include "CardStack.h"
#include "CardVBoxLayout.h"
#include "ClickableLabel.h"


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

    QHBoxLayout* getHBoxLayout();

public slots:
    void showNextCard();

private:
    // Box layout to display the cards
    CCardVBoxLayout* vbox;
    QHBoxLayout* boxLayout;

    // Holds the index of the current visible card
    int currentCard = 0;

    // Label that represents the unflipped drawStack but needs no logic
    CClickableLabel* drawStackPlaceholder;

    // The pixmap of the card back and empty drawStack placeholder
    QPixmap cardBackPixmap;
    QPixmap emptyDrawStackPixmap;
};


