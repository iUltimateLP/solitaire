#ifndef WINSCREEN_H
#define WINSCREEN_H

#include <QWidget>

namespace Ui {
class WinScreen;
}

class WinScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WinScreen(int windowW, int windowH, int windowX, int windowY, QWidget *parent = nullptr);
    ~WinScreen();

private:
    // Overwritten mouse events
    void mousePressEvent(QMouseEvent* ev) override;

private:
    Ui::WinScreen *ui;
};

#endif // WINSCREEN_H
