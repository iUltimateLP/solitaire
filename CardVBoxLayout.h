// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QLayout>
#include <QVector>

// An own QLayout implementation, similar to a QVBoxLayout, but with overlapping elements
// See: https://doc.qt.io/qt-5/layout.html
class CCardVBoxLayout : public QLayout
{
public:
    // Constructor
    CCardVBoxLayout(int spacing) : QLayout() { setSpacing(spacing); }
    CCardVBoxLayout(int spacing, QWidget *parent) : QLayout(parent) { setSpacing(spacing); }

    // Destructor
    ~CCardVBoxLayout();

    //~ Begin QLayout interface
    void addItem(QLayoutItem *item) override;
    QSize sizeHint() const override;
    QSize minimumSize() const override;
    int count() const override;
    QLayoutItem *itemAt(int) const override;
    QLayoutItem *takeAt(int) override;
    void setGeometry(const QRect &rect) override;
    //~ End QLayout interface

private:
    QVector<QLayoutItem*> m_items;
};
