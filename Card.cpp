// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "Card.h"
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include "Main.h"

// Set cardTileSize and cardSizeFactor
QSize CCard::cardTileSize = QSize(170, 255);
float CCard::cardSizeFactor = 0.6;

Q_DECL_IMPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0); // src/widgets/effects/qpixmapfilter.cpp

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

void CCard::mousePressEvent(QMouseEvent* ev)
{
    // If the left mouse button was pressed and is flipped
    if (ev->button() == Qt::LeftButton && this->getFlipped())
    {
        // Create a new QDrag object to enable drag and drop
        QDrag* drag = new QDrag(this);

        // Every drag'n'drop action needs MIME data to identify whether drops are allowed at certain
        // places or not.
        QMimeData* mimeData = new QMimeData();

        // This however also gives us the option to transmit any payload we want with the drag'n'drop
        // operation. Here, we're inventing a application/x-card-data mime-type and simply serialize
        // the pointer to this card in the payload
        QByteArray dndPayload;
        dndPayload.fromRawData(reinterpret_cast<char*>(this), sizeof(CCard*));
        mimeData->setData("application/x-card-data", dndPayload);

        // Apply the new mime data and set the pixmap image to show when dragging this card
        drag->setMimeData(mimeData);
        drag->setPixmap(*this->pixmap());

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

        QPainter painter;
        QPixmap temp = drag->pixmap();
        painter.begin(&temp);
        painter.setCompositionMode(QPainter::CompositionMode_Multiply);
        painter.fillRect(temp.rect(), QColor(0,0,0,127));
        painter.end();
        drag->setPixmap(temp);

        // Start the drag and drop action
        Qt::DropAction dropAction = drag->exec();
        Q_UNUSED(dropAction);
    }
}

void CCard::mouseReleaseEvent(QMouseEvent* ev)
{
    Q_UNUSED(ev);
}

void CCard::enterEvent(QEvent* ev)
{
    Q_UNUSED(ev);
}

void CCard::leaveEvent(QEvent* ev)
{
    Q_UNUSED(ev);
}
