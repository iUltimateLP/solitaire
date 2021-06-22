// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Card.h"
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QApplication>
#include "Main.h"

// Set cardTileSize and cardSizeFactor
QSize CCard::cardTileSize = QSize(170, 255);
float CCard::cardSizeFactor = 0.6;

// Global streaming operator to serialzie a CardDndPayload into a data stream
QDataStream& operator<<(QDataStream& s, const CardDndPayload* payloadPtr)
{
    // Just serialize the pointer
    qulonglong ptrval(*reinterpret_cast<qulonglong*>(&payloadPtr));
    return s << ptrval;
}

// Global streaming operator to deserialize a CardDndPayload from a data stream
QDataStream& operator>>(QDataStream& s, CardDndPayload*& payloadPtr)
{
    // Deserialize the pointer value from the stream
    qulonglong ptrval;
    s >> ptrval;

    // Reinterpret the pointer
    payloadPtr = *reinterpret_cast<CardDndPayload**>(&ptrval);
    return s;
}

CCard::CCard(QWidget *parent, const ECardSymbol symbol, const ECardType type, const int numberValue)
    : QLabel(parent)
    , cardSymbol(symbol)
    , cardType(type)
    , cardNumberValue(numberValue)
    , isFlipped(true)
{
    // Load the pixmap of the cards tileset
    QPixmap tileset(":/assets/card_tileset.png");

    // Calculate the position of the card on the tileset
    int tilesetX = (type == ECardType::Number ? numberValue - 2 : (int)type);
    int tilesetY = (int)symbol;

    // Extract that region off the tileset
    // We're creating this pixmap on the heap using the copy constructor of QPixmap(const QPixmap&) so we have access to it later
    cardFrontPixmap = new QPixmap(tileset.copy(tilesetX * cardTileSize.width(), tilesetY * cardTileSize.height(), cardTileSize.width(), cardTileSize.height()));

    // Create the pixmap for the back of the card
    cardBackPixmap = new QPixmap(":/assets/card_back.png");

    // Flipped by default, also applys the pixmap
    this->setCardFlipped(true);

    // Apply the screen size to this whole widget's geometry
    this->setGeometry(0, 0, cardTileSize.width(), cardTileSize.height());
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Hide the background which this card (a QLabel-child) will inherit from it's parent (the MainWindow)
    this->setStyleSheet("background-color: rgba(0, 0, 0, 0); background: transparent;");
}

QSize CCard::getCardScreenSize()
{
    return cardTileSize * cardSizeFactor;
}

void CCard::setCardFlipped(bool shouldFlip)
{
    // Set the variable
    this->isFlipped = shouldFlip;

    // Apply the correct pixmap
    QPixmap pixmap = isFlipped ? *cardFrontPixmap : *cardBackPixmap;

    // Set the card image to the container, scaled by the desired screensize of the card
    this->setPixmap(pixmap.scaled(getCardScreenSize().width(), getCardScreenSize().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Create a drop shadow effect to make it easier to distinguish cards on one stack
    QGraphicsDropShadowEffect* DropShadowEffect = new QGraphicsDropShadowEffect();
    DropShadowEffect->setBlurRadius(50);
    DropShadowEffect->setOffset(0, 0);
    DropShadowEffect->setColor(QColor(0, 0, 0, 64));
    this->setGraphicsEffect(DropShadowEffect);
}

QString CCard::toString()
{
    QString str;
    QTextStream stream(&str);

    // Format type and symbol strings
    QString typeStr, symbolStr;
    switch (getType()) {
        case ECardType::Number: typeStr = "Number";  break;
        case ECardType::Jack:   typeStr = "Jack";    break;
        case ECardType::King:   typeStr = "King";    break;
        case ECardType::Queen:  typeStr = "Queen";   break;
        case ECardType::Ace:    typeStr = "Ace";     break;
    }
    switch (getSymbol()) {
        case ECardSymbol::Heart:   symbolStr = "Heart";   break;
        case ECardSymbol::Diamond: symbolStr = "Diamond"; break;
        case ECardSymbol::Spade:   symbolStr = "Spade";   break;
        case ECardSymbol::Club:    symbolStr = "Club";    break;
    }

    // Create final string
    stream << "(Card " << this << " type=" << typeStr << " symbol=" << symbolStr << " numVal=" << getNumberValue() << " overallValue=" << getOverallValue() << ")";
    return str;
}

void CCard::mousePressEvent(QMouseEvent* ev)
{
    // If the left mouse button was pressed and is flipped
    if (ev->button() == Qt::LeftButton && this->getFlipped())
    {
        // Remember this position
        dragStartPos = ev->pos();
    }
}

void CCard::mouseReleaseEvent(QMouseEvent* ev)
{
    Q_UNUSED(ev);
}

void CCard::mouseMoveEvent(QMouseEvent* ev)
{
    // Make sure the left button is held and we dragged a minimum distance to further handle the dragging
    if (!(ev->buttons() & Qt::LeftButton) || (ev->pos() - dragStartPos).manhattanLength() < 1 || !this->getFlipped()) return;

    // Create a new QDrag object to enable drag and drop
    QDrag* drag = new QDrag(this);

    // Every drag'n'drop action needs MIME data to identify whether drops are allowed at certain
    // places or not.
    QMimeData* mimeData = new QMimeData();
    CardDndPayload* payload = new CardDndPayload();

    // Try to cast the stack this card is currently on to a holding stack
    CHoldingStack* myHoldingStack = dynamic_cast<CHoldingStack*>(this->getCardStack());

    // If this card is in a holding stack
    if (myHoldingStack)
    {
        // Get all cards above this card
        QList<CCard*> cardsAboveMe = myHoldingStack->getCardsAbove(this);

        // Draw a pixmap containing all cards of the "substack"
        QPixmap dragPixmap = QPixmap(this->pixmap()->width(), this->pixmap()->height() + ((cardsAboveMe.length() - 1) * 40));
        QPainter painter;
        painter.begin(&dragPixmap);

        for (int i = 0; i < cardsAboveMe.length(); i++)
        {
            CCard* c = cardsAboveMe[i];
            painter.drawPixmap(0, i * 40, c->pixmap()->width(), c->pixmap()->height(), *c->pixmap());

            // Add this card to the payload
            payload->cards.push_back(c);
        }

        // Not a single card
        payload->isSingleCard = false;

        // Stop painting and set the pixmap
        painter.end();
        drag->setPixmap(dragPixmap);
    }
    else
    {
        // Just draw the pixmap of this card
        drag->setPixmap(*this->pixmap());

        // Add this single card to the dnd payload
        payload->cards.push_back(this);
        payload->isSingleCard = true;
    }

    // Serialize the payload into the MIME data
    QByteArray payloadData;
    QDataStream payloadDataStream(&payloadData, QIODevice::WriteOnly);
    payloadDataStream << payload;
    mimeData->setData("application/x-solitaire-dnd", payloadData);

    // Apply the new mime data and set the pixmap image to show when dragging this card
    drag->setMimeData(mimeData);

    // ev->pos() is relative to the pivot point of the card (topleft)
    // this->pos() is relative to the parent widget (the card stack)
    // Therefore we need to map these coordinates first to a common coordinate origin, which is
    // the main window in this case
    QPoint mousePosInWindow = this->mapTo(CMain::get()->getGameWindow(), ev->pos());
    QPoint cardPosInWindow = this->mapTo(CMain::get()->getGameWindow(), this->pos());

    // The "hot spot" is the offset in pixels used to offset the pixmap from the mouse cursor when
    // dragging. Subtracting this->pos() from the cardPosInWindow makes sure that the offset of the
    // card in it's parent container (the stacks) is not taken into account. More on that here:
    // https://doc.qt.io/archives/qt-4.8/qdrag.html
    QPoint dndHotSpot = mousePosInWindow - (cardPosInWindow - this->pos());
    drag->setHotSpot(dndHotSpot);

    // Paint a gray overlay to indicate this is a card we're dragging
    // TODO: a better visual effect?
    QPainter painter;
    QPixmap temp = drag->pixmap();
    painter.begin(&temp);
    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
    painter.fillRect(temp.rect(), QColor(0, 0, 0, 127));
    painter.end();
    drag->setPixmap(temp);

    // Start the drag and drop action. This call is blocking!
    Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
    Q_UNUSED(dropAction);
}

void CCard::enterEvent(QEvent* ev)
{
    Q_UNUSED(ev);
}

void CCard::leaveEvent(QEvent* ev)
{
    Q_UNUSED(ev);
}
