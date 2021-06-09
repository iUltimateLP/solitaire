// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Card.h"
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

// Set cardTileSize and cardSizeFactor
QSize CCard::cardTileSize = QSize(170, 255);
float CCard::cardSizeFactor = 0.6;

CCard::CCard(QWidget *parent, const ECardSymbol symbol, const ECardType type, const int numberValue)
    : QLabel(parent)
    , cardSymbol(symbol)
    , cardType(type)
    , cardNumberValue(numberValue)
{
    // Load the pixmap of the cards tileset
    QPixmap tileset(":/assets/card_tileset.png");

    // Calculate the position of the card on the tileset
    int tilesetX = (type == ECardType::Number ? numberValue - 2 : (int)type);
    int tilesetY = (int)symbol;

    // Extract that region off the tileset
    QPixmap cardTile = tileset.copy(tilesetX * cardTileSize.width(), tilesetY * cardTileSize.height(), cardTileSize.width(), cardTileSize.height());

    // Set the card image to the container, scaled by the desired screensize of the card
    this->setPixmap(cardTile.scaled(getCardScreenSize().width(), getCardScreenSize().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Apply the screen size to this whole widget's geometry
    this->setGeometry(0, 0, cardTileSize.width(), cardTileSize.height());
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Create a drop shadow effect to make it easier to distinguish cards on one stack
    QGraphicsDropShadowEffect* DropShadowEffect = new QGraphicsDropShadowEffect();
    DropShadowEffect->setBlurRadius(50);
    DropShadowEffect->setOffset(0, 0);
    DropShadowEffect->setColor(QColor(0, 0, 0, 64));
    this->setGraphicsEffect(DropShadowEffect);
}

QSize CCard::getCardScreenSize()
{
    return cardTileSize * cardSizeFactor;
}

void CCard::mousePressEvent(QMouseEvent* ev)
{
    qDebug() << "Clicked card" << this->cardSymbol << "," << this->cardType << "," << this->cardNumberValue;
}

void CCard::mouseReleaseEvent(QMouseEvent* ev)
{
    qDebug() << "Released card" << this->cardSymbol << "," << this->cardType << "," << this->cardNumberValue;
}

void CCard::enterEvent(QEvent* ev)
{
    qDebug() << "Hover card" << this->cardSymbol << "," << this->cardType << "," << this->cardNumberValue;
}

void CCard::leaveEvent(QEvent* ev)
{
    qDebug() << "Leave card" << this->cardSymbol << "," << this->cardType << "," << this->cardNumberValue;
}
