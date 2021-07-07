// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "DrawStack.h"
#include "Main.h"
#include "CardVBoxLayout.h"
#include <QDebug>

CDrawStack::CDrawStack(QWidget* parent)
    : CCardStack(parent)
{
    //TODO: why do we need setGeometry? -> does not change anything when commented out
    // This widget has always the size of a card
    //this->setGeometry(0, 0, CCard::cardTileSize.width(), CCard::cardTileSize.height());

    // Create the dummy vbox layout with zero spacing, so they get overlayed
    hbox = new CCardHBoxLayout(20, this);

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
    drawStackPlaceholder = new CClickableLabel(this);
    drawStackPlaceholder->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    drawStackPlaceholder->setPixmap(cardBackPixmap);

    // Add the playeholder and the cards to the boxLayout
    boxLayout->addWidget(drawStackPlaceholder);
    boxLayout->addWidget(this);

    QObject::connect(drawStackPlaceholder, &CClickableLabel::clicked, this, &CDrawStack::showNextCard);
}

void CDrawStack::addCard(CCard* cardToAdd)
{
    CCardStack::addCard(cardToAdd);
}

void CDrawStack::removeCard(CCard* cardToRemove)
{

    // Call the superclasses' removeCard
    CCardStack::removeCard(cardToRemove);
    removeCardFromUi();
}

bool CDrawStack::canDropCard(CCard *cardToDrop)
{
    Q_UNUSED(cardToDrop);

    // Adding a card to the drawStack is not possible, due to that, this method is not necessary
    qDebug() << "nothing happens";

    return true;
}

CCard* CDrawStack::getTopCard()
{
    return getCards()[currentCard];
}

void CDrawStack::addCardToUi()
{
    // When the Boxlayout shows already 3 cards, the last one is removed, so that a new one can be displayed
    if(hbox->count() == 3 && getNumCards() > 3)
    {
        hbox->removeItem(hbox->itemAt(0));
        getCards()[currentCard+3]->setVisible(false);
    }

    // The following card is added to the boxlayout and set to visible
    hbox->addWidget(getCards()[currentCard]);
    getCards()[currentCard]->setVisible(true);

    // Interaction with the added card should be possible, with the former front card impossible (only if there is a former card)
    getCards()[currentCard]->setCanInteract(true);
    if(currentCard < getNumCards()-1)
       getCards()[currentCard+1]->setCanInteract(false);

    // Additionally, if it is the last card of the stack or if there is no card left, the placeholder displays the empty tile
    if(currentCard == 0 || getNumCards()-1 == 0)
        drawStackPlaceholder->setPixmap(emptyDrawStackPixmap);
}

void CDrawStack::removeCardFromUi()
{
    // Remove the card from the layout which is the first one
    if(hbox->itemAt(2))
        hbox->removeItem(hbox->itemAt(2));
    else if(hbox->itemAt(1))
        hbox->removeItem(hbox->itemAt(1));
    else
        hbox->removeItem(hbox->itemAt(0));

    // The next card should be interactable, if there is one
    if(currentCard <= getNumCards()-1)
    {
        qDebug() << "next card is set to interact";
        getCards()[currentCard]->setCanInteract(true);
        // When a card is removed, the last third card in the stack will be displayed, if existent
        if(currentCard + 2 <= getNumCards()-1)
        {
            qDebug() << "next card should be displayed";
            hbox->push_back(getCards()[currentCard+2]);
            getCards()[currentCard+2]->setVisible(true);
        }
    }
}

void CDrawStack::showNextCard()
{
    qDebug() << "current card" << currentCard;
    // The cards that shouldn't be displayed have to be invisible, even if the card is removed from
    // the layout (the card is still in the background of the layout)
    // If there are cards left on the stack, the variable "currentCard" is decremented
    if(currentCard > 0)
    {
        --currentCard;
        addCardToUi();
    }
    else if(currentCard == 0)
    {
        qDebug() << "set to last card";

        // CurrentCard = 0 means, that the stack is empty, so the drawStack gets recycled
        drawStackPlaceholder->setPixmap(cardBackPixmap);

        // Recycling the draw stack changes the score
        CMain::get()->getGameInstance()->addScore(GameScoringAttributes::RECYCLING_DRAW_PILE);

        // Iterating over the remaining cards and setting them to invisible and not interactable
        for(int i = 0; i < hbox->getItemCount(); ++i)
        {
            getCards()[currentCard + i]->setCanInteract(false);
            getCards()[currentCard + i]->setVisible(false);
        }

        hbox->clear();

        // Decrementing currentCard again, so that we don't jump in the same condition next time
        --currentCard;
    }
    // Otherwise, the currentCard is set to the topCard again, the first card is set to visible and caninteract
    else if(getNumCards() > 0)
    {
        currentCard = getNumCards()-1;
        addCardToUi();
    }

    // Register a new transaction that we drew a card
    Transaction t;
    t.type = Transaction::TransactionType::DrawFromDrawStack;
    t.stack1 = this;

    CMain::get()->getGameInstance()->addTransaction(t);

    // Drawing a card counts as a move
    CMain::get()->getGameWindow()->incrementMove();

    // Play a sound effect
    CMain::get()->getSoundManager()->playSoundEffect(SoundEffectType::CardStack);
}

void CDrawStack::undo(CCard* card)
{
    // If a card has to be readded to the drawstack
    if(card)
    {
       insertCardAt(currentCard, card);
       addCardToUi();
    }
    else
    {
        // If the recycling has to be undone:
        if (currentCard == -1)
        {
            // TODO: Beautify
            currentCard = 2;
            addCardToUi();
            currentCard = 1;
            addCardToUi();
            currentCard = 0;
            addCardToUi();
            drawStackPlaceholder->setPixmap(cardBackPixmap);
        }
        else
        {
            getCards()[currentCard]->setVisible(false);
            ++currentCard;
            removeCardFromUi();
        }
    }
}

QHBoxLayout* CDrawStack::getHBoxLayout()
{
    return boxLayout;
}
