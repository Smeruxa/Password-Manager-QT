#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "libraries.h"

class AnimationManager : public QObject
{
    Q_OBJECT

public:
    void addAnimation(QObject *object, const QColor &defaultColor, const QColor &changeColor);
    QColor hexStringToQColor(const QString& hexString);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QHash<QObject*, QVector<QColor>> originalColors;
};

#endif // ANIMATIONMANAGER_H
