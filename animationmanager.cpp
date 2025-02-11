#include "animationmanager.h"

QColor AnimationManager::hexStringToQColor(const QString& hexString) {
    if (hexString.length() != 7 || hexString[0] != '#') {
        return QColor();
    }

    bool conversionOk;
    int red = hexString.mid(1, 2).toInt(&conversionOk, 16);
    if (!conversionOk) {
        return QColor();
    }

    int green = hexString.mid(3, 2).toInt(&conversionOk, 16);
    if (!conversionOk) {
        return QColor();
    }

    int blue = hexString.mid(5, 2).toInt(&conversionOk, 16);
    if (!conversionOk) {
        return QColor();
    }

    return QColor(red, green, blue);
}

void AnimationManager::addAnimation(QObject *object, const QColor &defaultColor, const QColor &changeColor)
{
    if (object)
    {
        originalColors[object].resize(2);
        originalColors[object][0] = defaultColor; // Цвет по умолчанию
        originalColors[object][1] = changeColor; // Цвет для изменения

        object->installEventFilter(this);
    }
}

bool AnimationManager::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *widget = qobject_cast<QWidget*>(obj);
    if (!widget) {
        return QObject::eventFilter(obj, event);
    }

    if (originalColors.contains(obj)) {
        if (event->type() == QEvent::Enter) {
            // Создание анимации изменения цвета
            QGraphicsColorizeEffect *colorizeEffect = new QGraphicsColorizeEffect();
            QPropertyAnimation *colorAnimation = new QPropertyAnimation(colorizeEffect, "color");
            colorAnimation->setStartValue(originalColors[obj][0]);
            colorAnimation->setEndValue(originalColors[obj][1]); // Изменить на нужный цвет
            colorAnimation->setDuration(100);  // Установите длительность анимации в миллисекундах
            colorAnimation->start();

            widget->setGraphicsEffect(colorizeEffect);
        }
        else if (event->type() == QEvent::Leave) {
            QGraphicsColorizeEffect *colorizeEffect = new QGraphicsColorizeEffect();
            QPropertyAnimation *colorAnimation = new QPropertyAnimation(colorizeEffect, "color");
            colorAnimation->setStartValue(originalColors[obj][1]);
            colorAnimation->setEndValue(originalColors[obj][0]);
            colorAnimation->setDuration(100);
            colorAnimation->start();

            connect(colorAnimation, &QPropertyAnimation::finished, this, [widget]() {
                widget->setGraphicsEffect(nullptr);
            });

            widget->setGraphicsEffect(colorizeEffect);
        }
    }

    return QObject::eventFilter(obj, event);
}
