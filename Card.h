// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include <QWidget>
#include <QLabel>

// Forward declaration
class CCardStack;

// Card symbol enumeration
enum ECardSymbol
{
    Heart,
    Diamond,
    Spade,
    Club
};

// Card type enumeration
// For simplicity, we assign index values to the enum entries which are matching with the X positions
// in the tileset, so we can easily convert a ECardType to an integer and then we got the X position
enum ECardType
{
    Number = 0,
    Jack   = 9,
    Queen  = 10,
    King   = 11,
    Ace    = 12
};

// Payload data used for drag-and-drop operations
struct CardDndPayload
{
public:
    // A list of cards which were dragged
    QList<class CCard*> cards;

    // Whether this was a single card or a card stack
    bool isSingleCard;

    // Streaming operators so we can serialize this struct
    // Taken from here: https://stackoverflow.com/questions/24345681/qdatastream-serialize-pointer
    friend QDataStream& operator<<(QDataStream& s, const CardDndPayload* payloadPtr);
    friend QDataStream& operator>>(QDataStream& s, CardDndPayload*& payloadPtr);
};

// Implements a playing card
class CCard : public QLabel
{
    Q_OBJECT

public:
    // The size of a card on the tileset
    static QSize cardTileSize;

    // The factor by which the cardTileSize get's multiplied for screen size
    static float cardSizeFactor;

public:
    // Constructor
    CCard(QWidget *parent = nullptr, const ECardSymbol symbol = ECardSymbol::Heart, const ECardType type = ECardType::Number, const int numberValue = 0);

public:
    // Get the actual screen size of a card in pixels
    static QSize getCardScreenSize();

    // Returns the symbol of this card
    ECardSymbol getSymbol() const
    {
        return cardSymbol;
    }

    // Returns the type of this card
    ECardType getType() const
    {
        return cardType;
    }

    // Returns the number value of this card (if card type == ECardType::Number)
    int getNumberValue() const
    {
        return cardNumberValue;
    }

    // Returns whether this card is red (true) or black (false)
    bool isRed() const
    {
        // Hearts and Diamonds are red
        return (cardSymbol == ECardSymbol::Heart || cardSymbol == ECardSymbol::Diamond);
    }

    // Returns the overall "value" of the card - being 2..10 for numbers, 11 for jack, 12 for queen, 13 for king, 14 for ace
    int getOverallValue() const
    {
        // If the card type is a number, just return the number value
        if (cardType == ECardType::Number)
        {
            return cardNumberValue;
        }
        else
        {
            // We can convert the type enum to int, because we assigned custom integers to the enum. We have to add 2 though,
            // as the enum doesn't start counting at 2 (like the cards actually do)
            return ((int)cardType) + 2;
        }
    }

    // Sets whether this card is flipped or not (true = you see the image, false = you see the back)
    void setCardFlipped(bool shouldFlip);

    // Returns whether this card is flipped
    bool getFlipped()
    {
        return isFlipped;
    }

    // Sets the stack this card is in
    void setCardStack(CCardStack* newStack)
    {
        currentStack = newStack;
    }

    // Returns the stack this card is on
    CCardStack* getCardStack()
    {
        return currentStack;
    }

    // Converts this card to a human readable string
    QString toString();

private:
    // Overwritten mouse events
    void mousePressEvent(QMouseEvent* ev) override;
    void mouseReleaseEvent(QMouseEvent* ev) override;
    void mouseMoveEvent(QMouseEvent* ev) override;
    void enterEvent(QEvent* ev) override;
    void leaveEvent(QEvent* ev) override;

private:
    // This card's symbol and type
    ECardSymbol cardSymbol;
    ECardType cardType;
    int cardNumberValue;

    // Whether this card is flipped
    bool isFlipped;

    // The pixmap of the card front
    QPixmap* cardFrontPixmap;
    QPixmap* cardBackPixmap;

    // The stack the card is in right now
    CCardStack* currentStack;

    // The position where we started clicking so we know whether it
    // was a drag request or not
    QPoint dragStartPos;
};
