#ifndef SHOWEDIT_H
#define SHOWEDIT_H

#include "libraries.h"
#include "templates.h"
#include "configmanager.h"
#include "font.h"
#include "objectselect.h"

class ShowEdit : public QObject
{
    Q_OBJECT
public:
    explicit ShowEdit(QVBoxLayout* layout = nullptr, ConfigManager* config = nullptr, szFont* font = nullptr, ObjectSelect* selectBox = nullptr);

    template<typename T>
    void initPage(int index);

    template<typename T>
    QTextEdit* mutableMultiline(QHBoxLayout* secondLayout, T structure, int index, QString& field, int fieldIndex);

    template<typename T>
    QLineEdit* mutableString(QHBoxLayout* secondLayout, T structure, int index, QString& field, int fieldIndex, bool isBlue);

private:
    QHash<QString, bool> isEditing;

    QVBoxLayout* mainLayout;
    ConfigManager* config;
    ObjectSelect* selectBox;
    szFont* iFont;

    unsigned int COUNT_EDITS = 0;
    unsigned int COUNT_MULTILINE_EDITS = 0;
};

#endif // SHOWEDIT_H
