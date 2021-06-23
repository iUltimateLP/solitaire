// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "CardHBoxLayout.h"
#include <QDebug>
#include "Card.h"

int CCardHBoxLayout::count() const
{
    // QVector::size() returns the number of QLayoutItems in m_items
    return m_items.size();
}

QLayoutItem* CCardHBoxLayout::itemAt(int idx) const
{
    // QVector::value() performs index checking, and returns nullptr if we are
    // outside the valid range
    return m_items.value(idx);
}

QLayoutItem* CCardHBoxLayout::takeAt(int idx)
{
    // QVector::take does not do index checking
    return idx >= 0 && idx < m_items.size() ? m_items.takeAt(idx) : 0;
}

void CCardHBoxLayout::addItem(QLayoutItem *item)
{
    m_items.append(item);
}

CCardHBoxLayout::~CCardHBoxLayout()
{
    // Delete all items
    QLayoutItem* item;
    while ((item = takeAt(0)))
        delete item;
}

void CCardHBoxLayout::setGeometry(const QRect &r)
{
    // First layout the parent geometry
    QLayout::setGeometry(r);

    // Don't do anything if no elements
    if (m_items.size() == 0)
        return;

    // Calculate the width and height
    int i = 0;

    // Iterate all over items
    while (i < m_items.size())
    {
        // Calculate the geometry per item
        QLayoutItem* o = m_items.at(i);
        QRect geom(r.x() + (i * spacing()), r.y(), CCard::getCardScreenSize().width(), CCard::getCardScreenSize().height());
        o->setGeometry(geom);
        ++i;
    }
}

QSize CCardHBoxLayout::sizeHint() const
{
    QSize s(0, 0);
    int n = m_items.count();

    // If we have more than 0 items
    if (n > 0)
        s = CCard::getCardScreenSize(); // Default size

    // Iterate over items and expand all item's sizes
    int i = 0;
    while (i < n) {
        QLayoutItem *o = m_items.at(i);
        s = s.expandedTo(o->sizeHint());
        ++i;
    }

    // Calculate final size
    return s + n * QSize(spacing(), 0);
}

QSize CCardHBoxLayout::minimumSize() const
{
    // Iterate over all items and calculate their minimum size
    QSize s(0, 0);
    int n = m_items.count();
    int i = 0;
    while (i < n) {
        QLayoutItem *o = m_items.at(i);
        s = s.expandedTo(o->minimumSize());
        ++i;
    }
    return s + n * QSize(spacing(), 0);
}

void CCardHBoxLayout::clear()
{
    this->removeItem(m_items[2]);
       this->removeItem(m_items[1]);
        this->removeItem(m_items[0]);
    m_items.clear();
}

