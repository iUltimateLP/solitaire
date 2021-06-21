// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
#include "ClickableLabel.h"

CClickableLabel::CClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent)
{

}

void CClickableLabel::mousePressEvent(QMouseEvent* ev)
{
    // Simply emit the signal
    emit clicked();
}
