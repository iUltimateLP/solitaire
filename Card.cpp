// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Card.h"
#include <QDebug>

CCard::CCard(QWidget *parent, ECardSymbol symbol, ECardType type, int numberValue)
    : QWidget(parent)
    , cardSymbol(symbol)
    , cardType(type)
    , cardNumberValue(numberValue)
{
    // Create the QLabel
    imageContainer = new QLabel(this);

    // Load the pixmap of the cards tileset
    QPixmap tileset(":/assets/card_tileset.png");

    // Calculate the position of the card on the tileset
    int tilesetX = (type == ECardType::Number ? numberValue - 2 : (int)type);
    int tilesetY = (int)symbol;
    int cardSizeX = 170;
    int cardSizeY = 255;

    // The size of the card on the screen
    int cardScreenSizeX = cardSizeX * 0.6;
    int cardScreenSizeY = cardSizeY * 0.6;

    // Extract that region off the tileset
    QPixmap cardTile = tileset.copy(tilesetX * cardSizeX, tilesetY * cardSizeY, cardSizeX, cardSizeY);

    // Set the card image to the container, scaled by the desired screensize of the card
    imageContainer->setPixmap(cardTile.scaled(cardScreenSizeX, cardScreenSizeY, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Apply the screen size to this whole widget's geometry
    this->setGeometry(this->pos().x(), this->pos().y(), cardScreenSizeX, cardScreenSizeY);
}
