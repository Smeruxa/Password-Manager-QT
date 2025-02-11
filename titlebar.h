#ifndef TITLEBAR_H
#define TITLEBAR_H

#include "libraries.h"
#include "animationmanager.h"
#include "font.h"

class TitleBar : public QObject
{
    Q_OBJECT
public:
    TitleBar();

    QFrame* getMainFrame() const;
    QPushButton* getMaximizeButton();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void hideMainWindow();
    void restoreMainWindow();
    void closeMainWindow();

private slots:
    void startDrag(QMouseEvent *event);
    void dragMove(QMouseEvent *event);

private:
    szFont* iFont;
    QPoint dragPosition;
    QFrame* mainFrame;

    QPushButton* hideButton;
    QPushButton* maximizeButton;
    QPushButton* closeButton;

    AnimationManager* animationManager;
};

#endif // TITLEBAR_H
