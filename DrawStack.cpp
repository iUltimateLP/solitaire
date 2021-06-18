// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "DrawStack.h"
#include "CardVBoxLayout.h"
#include <QDebug>

#include <QPushButton>
#include <Main.h>

CDrawStack::CDrawStack(QWidget* parent)
    : CCardStack(parent)
{
    //TODO: why do we need setGeometry? -> does not change anything when commented out
    // This widget has always the size of a card
    //this->setGeometry(0, 0, CCard::cardTileSize.width(), CCard::cardTileSize.height());

    // Create the dummy vbox layout with zero spacing, so they get overlayed
    vbox = new CCardVBoxLayout(0, this);

    // Create the boxLayout that contains the playholder and the cards
    boxLayout = new QHBoxLayout();

    // Set the space between the cards
    boxLayout->setSpacing(10);

    // Assign and scale the needed tiles to the pixmap variables
    QPixmap* cardBackTile = new QPixmap(":/assets/card_back.png");
    cardBackPixmap = cardBackTile->scaled(CCard::getCardScreenSize().width(), CCard::getCardScreenSize().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap placeholderTilset = QPixmap(":/assets/card_placeholders.png");
    QPixmap placeholderTile = placeholderTilset.copy(4 * CCard::cardTileSize.width(), 0, CCard::cardTileSize.width(), CCard::cardTileSize.height());
    emptyDrawStackPixmap = placeholderTile.scaled(CCard::getCardScreenSize().width(), CCard::getCardScreenSize().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Create a QLabel that represents the drawStack but needs no logic
    drawStackPlaceholder = new ClickableLabel(this);
    drawStackPlaceholder->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    drawStackPlaceholder->setPixmap(cardBackPixmap);

   // boxLayout->setContentsMargins(0,0,0,0);

    // Add the playeholder and the cards to the boxLayout
    boxLayout->addWidget(drawStackPlaceholder);
    boxLayout->addWidget(this);

    QObject::connect(drawStackPlaceholder, &ClickableLabel::clicked, this, &CDrawStack::showNextCard);
    //addCard()
}

void CDrawStack::addCard(CCard* cardToAdd)
{
    CCardStack::addCard(cardToAdd);
    //boxLayout->addWidget(cardToAdd)5
    vbox->addWidget(cardToAdd);
}

void CDrawStack::removeCard(CCard* cardToRemove)
{
    // Call the superclasses' removeCard
    CCardStack::removeCard(cardToRemove);

    // When a card is removed, the next card in the stack will be displayed
    --currentCard;
    if(currentCard > 0)
    {
       getCards()[currentCard]->setVisible(1);
    }

    vbox->removeWidget(cardToRemove);

}

bool CDrawStack::canDropCard(CCard *cardToDrop)
{
    // Adding a card to the drawStack is not possible, due to that, this method is not necessary
    qDebug() << "nothing happens";
}

void CDrawStack::showNextCard()
{
    // The current card is set to invisible
    getCards()[currentCard]->setVisible(0);
    // If there are cards left on the stack, the variable "currentCard" gets decremented

    if(currentCard > 0 )
    {
        --currentCard;
        if(currentCard > 0 )
        {
            // If there are still cards after decrementing the variable, the following card is set to visible
            getCards()[currentCard]->setVisible(1);
            if(currentCard == 1)
            {
                // Additionally, if it is the last card, the placeholder displays the empty tile
                drawStackPlaceholder->setPixmap(emptyDrawStackPixmap);
            }
        }
        else
        {
            drawStackPlaceholder->setPixmap(cardBackPixmap);
        }
    }
    // Otherwise, the stack is recycled by setting the currentCard to the topCard again
    else
    {
        currentCard = getNumCards()-1;
        getCards()[currentCard]->setVisible(1);
    }
    qDebug() << "click and show next card";
}

QHBoxLayout* CDrawStack::getHBoxLayout()
{
    return boxLayout;
}
