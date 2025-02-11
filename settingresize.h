#ifndef SETTINGRESIZE_H
#define SETTINGRESIZE_H

#include "libraries.h"

class SettingResize : public QObject
{
    Q_OBJECT

public:
    explicit SettingResize(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void mousePress(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);

    Qt::CursorShape getCursorShape(const QPoint &pos) const;

private:
    QWidget *mainWindow;

    bool leftBorder;
    bool rightBorder;
    bool topBorder;
    bool bottomBorder;
    bool resizeStarted;
    bool cursorChanged;
    QPoint startPos;
    QRect startGeometry;

    int borderLength = 7;
};

#endif // SETTINGRESIZE_H
