// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QObject>
#include <QWidget>
#include <QLabel>

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

// Implements a playing card
class CCard : public QWidget
{
    Q_OBJECT

public:
    // Constructor
    CCard(QWidget *parent = nullptr, ECardSymbol symbol = ECardSymbol::Heart, ECardType type = ECardType::Number, int numberValue = 0);

private:
    // This card's symbol and type
    ECardSymbol cardSymbol;
    ECardType cardType;
    int cardNumberValue;

    // The QLabel image container to display the card
    QLabel* imageContainer;
};
