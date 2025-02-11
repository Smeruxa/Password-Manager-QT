#include "opacityhook.h"

OpacityHook::OpacityHook(QMainWindow* mainWindow) : QObject(mainWindow), mainWindow(mainWindow)
{
    mainWindow->installEventFilter(this);
    mainWindow->setAttribute(Qt::WA_TranslucentBackground);

    animation = new QPropertyAnimation(mainWindow, "windowOpacity");
    animation->setDuration(300);

    connect(animation, &QPropertyAnimation::valueChanged, this, [this](const QVariant &value) {
        setOpacity(value.toDouble());
    });

    connect(animation, &QPropertyAnimation::finished, this, [this]() {
        if (isHiding) {
            emit animationFinished();
        }
    });
}

OpacityHook::~OpacityHook()
{
    mainWindow->removeEventFilter(this);
    delete animation;
}

bool OpacityHook::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::Show && watched == mainWindow)
    {
        startAnimation(0.0, 1.0);
        isHiding = false;
    }

    return QObject::eventFilter(watched, event);
}

void OpacityHook::setOpacity(qreal opacity)
{
    mainWindow->setWindowOpacity(opacity);
}

void OpacityHook::startAnimation(qreal startValue, qreal endValue)
{
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->start();
}
