// Author: Annie Berend (5033782) - Jonathan Verbeek (5058288)
// Code from https://wiki.qt.io/Clickable_QLabel
#include <QLabel>
#include <QWidget>
#include <Qt>


class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableLabel();

signals:
    void clicked();

protected:
    void mousePressEvent (QMouseEvent* ev);
};
