#ifndef CGAMEWINDOW_H
#define CGAMEWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class CGameWindow; }
QT_END_NAMESPACE

class CGameWindow : public QMainWindow
{
    Q_OBJECT

public:
    CGameWindow(QWidget *parent = nullptr);
    ~CGameWindow();

private:
    Ui::CGameWindow *ui;
};
#endif // CGAMEWINDOW_H
