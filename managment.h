#ifndef MANAGMENT_H
#define MANAGMENT_H

#include "libraries.h"
#include "animationmanager.h"
#include "templates.h"
#include "font.h"
#include "configmanager.h"
#include "objectselect.h"
#include "createnewbox.h"
#include "showedit.h"

class Managment : public QObject
{
    Q_OBJECT
public:

    explicit Managment(QVBoxLayout *selectLayout = nullptr, QVBoxLayout *infoLayout = nullptr, QLabel *selectedNumber = nullptr);

    template<typename T>
    void createNewElement(STRUCTURES::TYPES type, const T& element);

private:

    szFont *iFont;

    QVBoxLayout *selectLayout;
    QVBoxLayout *infoLayout;
    QLabel *selectedNumber;

    QHBoxLayout* textFrameLayout;

    ObjectSelect* selectBox;

    int currentIndex = -1;
};

#endif // MANAGMENT_H
