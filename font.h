#ifndef FONT_H
#define FONT_H

#include "libraries.h"

class szFont : public QObject
{
    Q_OBJECT
public:
    szFont();

    QFont solid() { return faSolid; }
    QFont regular() { return faRegular; }
    QFont roboto() { return fontRoboto; }

    QFont resize(QFont, int);

private:
    QFont faSolid;
    QFont faRegular;
    QFont fontRoboto;

    void initializate();
};

#endif // FONT_H
