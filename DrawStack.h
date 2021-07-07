// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include "CardStack.h"
#include "CardHBoxLayout.h"
#include "ClickableLabel.h"

// The draw stack is the stack where new cards can be acquired from
// Acts like a circular buffer of cards
class CDrawStack : public CCardStack
{
    Q_OBJECT

public:
    // Default constructor
    CDrawStack(QWidget* parent = nullptr);

public:
    //~ Begin CCardStack interface
    virtual void addCard(CCard *cardToAdd) override;
    virtual void removeCard(CCard *cardToRemove) override;
    virtual bool canDropCard(CCard* cardToDrop) override;
    virtual CCard* getTopCard() override;
    //~ End CCardCard interface

    // Returns the hbox layout
    QHBoxLayout* getHBoxLayout();

    // When the undo Button is clicked and the last move was recycling the drawstack
    void undo(CCard* card = nullptr);

public slots:
    void showNextCard();

private:
    // Box layout to display the cards
    CCardHBoxLayout* hbox;
    QHBoxLayout* boxLayout;

    // Holds the index of the current visible card
    int currentCard = -1;

    // Label that represents the unflipped drawStack but needs no logic
    CClickableLabel* drawStackPlaceholder;

    // The pixmap of the card back and empty drawStack placeholder
    QPixmap cardBackPixmap;
    QPixmap emptyDrawStackPixmap;

    // Adds a new card to the UI
    void addCardToUi();

    // Removes the last card from the UI
    void removeCardFromUi();
};


