#ifndef CREATENEWBOX_H
#define CREATENEWBOX_H

#include "libraries.h"
#include "animationmanager.h"
#include "configmanager.h"
#include "objectselect.h"
#include "font.h"

class CreateNewBox : public QObject {
    Q_OBJECT
public:
    CreateNewBox(QLabel *parent, ConfigManager* config, ObjectSelect* selectBox);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QLabel* addButton;

    QDialog* mPopup;
    QHBoxLayout *mPopupLayout;

    int currentChoosed = -1;

    szFont* iFont;
    ConfigManager* config;
    ObjectSelect* selectBox;
};

#endif // CREATENEWBOX_H
