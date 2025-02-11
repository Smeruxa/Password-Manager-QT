#ifndef OPACITYHOOK_H
#define OPACITYHOOK_H

#include "libraries.h"

class OpacityHook : public QObject
{
    Q_OBJECT
public:
    OpacityHook(QMainWindow* mainWindow);
    ~OpacityHook();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void startAnimation(qreal startValue, qreal endValue);

    bool isHiding;

signals:
    void animationFinished();

private:
    void setOpacity(qreal opacity);

    QMainWindow* mainWindow;
    QPropertyAnimation* animation;
};

#endif // OPACITYHOOK_H
