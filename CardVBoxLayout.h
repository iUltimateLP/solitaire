// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QLayout>
#include <QVector>
#include <QVariantAnimation>

// An own QLayout implementation, similar to a QVBoxLayout, but with overlapping elements
// See: https://doc.qt.io/qt-5/layout.html
class CCardVBoxLayout : public QLayout
{
public:
    // Constructor
    CCardVBoxLayout(int spacing);
    CCardVBoxLayout(int spacing, QWidget *parent);

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

    // Sets the spacing with animation
    void setSpacingWithAnimation(int newSpacing);

private:
    // Vector of layout items to layout in this layout
    QVector<QLayoutItem*> m_items;

    // Animation to transition between spacings
    QVariantAnimation* spacingAnim;
};
