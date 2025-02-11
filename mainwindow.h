#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "libraries.h"
#include "titlebar.h"
#include "settingresize.h"
#include "opacityhook.h"
#include "managment.h"
#include "font.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    szFont* iFont;

    OpacityHook* opacityHook;
    TitleBar* titleBar;
    QPushButton *maximizeButton;
};
#endif
