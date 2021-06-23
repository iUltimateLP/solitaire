// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#pragma once

#include <QLabel>
#include <QWidget>

// A simple clickable QLabel implementation
// Code mostly taken from https://wiki.qt.io/Clickable_QLabel
class CClickableLabel : public QLabel
{
    Q_OBJECT

public:
    // Constructor
    CClickableLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

signals:
    // Fires when clicked
    void clicked();

protected:
    // Handles the mouse press event
    void mousePressEvent(QMouseEvent* ev);
};
