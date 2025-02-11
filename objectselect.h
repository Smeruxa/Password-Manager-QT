#ifndef OBJECTSELECT_H
#define OBJECTSELECT_H

#include "libraries.h"
#include "templates.h"
#include "font.h"
#include "configmanager.h"

class ObjectSelect : public QObject
{
    Q_OBJECT
public:
    explicit ObjectSelect(QVBoxLayout* parentLayout = nullptr, ConfigManager* config = nullptr, QVBoxLayout* infoLayout = nullptr);

    template<typename T>
    void addBox(T& structure, QChar symbolType);
    void refillLayout();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void clickedIndex(int index);
    void defaultLayout();

private:
    QString shortString(QString text, int maxLength);

    void createBox(QString mainText, QString additionalText, QChar symbolType);
    QVBoxLayout* parentLayout;
    QVBoxLayout* infoLayout;

    ConfigManager* config;
    szFont* iFont;

    unsigned int currentIndex = 0;
    unsigned int deleteIndex = 0;
};

#endif // OBJECTSELECT_H
