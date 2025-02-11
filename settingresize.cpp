#include "settingresize.h"

SettingResize::SettingResize(QWidget *parent)
    : QObject(parent), mainWindow(parent)
{
    if (mainWindow) {
        mainWindow->installEventFilter(this);
    }
}

bool SettingResize::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == mainWindow) {
        if (event->type() == QEvent::MouseButtonPress) {
            mousePress(static_cast<QMouseEvent*>(event));
        } else if (event->type() == QEvent::MouseMove) {
            mouseMove(static_cast<QMouseEvent*>(event));
        } else if (event->type() == QEvent::MouseButtonRelease) {
            mouseRelease(static_cast<QMouseEvent*>(event));
        }
    }

    return false;
}

void SettingResize::mousePress(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPos = event->globalPosition().toPoint();
        startGeometry = mainWindow->geometry();
        leftBorder = (event->pos().x() <= borderLength);
        rightBorder = (event->pos().x() >= mainWindow->width() - borderLength);
        topBorder = (event->pos().y() <= borderLength);
        bottomBorder = (event->pos().y() >= mainWindow->height() - borderLength);

        resizeStarted = (leftBorder || rightBorder || topBorder || bottomBorder);
        cursorChanged = false;
    }
}

void SettingResize::mouseMove(QMouseEvent *event)
{
    Qt::CursorShape cursorShape = getCursorShape(event->pos());

    if (resizeStarted) {
        int deltaX = event->globalPosition().x() - startPos.x();
        int deltaY = event->globalPosition().y() - startPos.y();

        QRect newGeometry = startGeometry;

        if (leftBorder) {
            newGeometry.setLeft(startGeometry.left() + deltaX);
        } else if (rightBorder) {
            newGeometry.setWidth(startGeometry.width() + deltaX);
        }

        if (topBorder) {
            newGeometry.setTop(startGeometry.top() + deltaY);
        } else if (bottomBorder) {
            newGeometry.setHeight(startGeometry.height() + deltaY);
        }

        if (newGeometry.height() < 50) {
            newGeometry.setTop(newGeometry.bottom() - 50);
        }

        mainWindow->setGeometry(newGeometry);

        if (!cursorChanged) {
            mainWindow->setCursor(cursorShape);
            cursorChanged = true;
        }
    } else {
        if (event->buttons() == Qt::NoButton && cursorShape != Qt::ArrowCursor) {
            mainWindow->setCursor(cursorShape);
        } else if (event->buttons() == Qt::LeftButton && (leftBorder || rightBorder || topBorder || bottomBorder)) {
            mainWindow->setCursor(cursorShape);
        } else {
            mainWindow->unsetCursor();
        }
    }
}

void SettingResize::mouseRelease(QMouseEvent *event)
{
    Q_UNUSED(event);
    leftBorder = false;
    rightBorder = false;
    topBorder = false;
    bottomBorder = false;

    mainWindow->unsetCursor();
    resizeStarted = false;
    cursorChanged = false;
}

Qt::CursorShape SettingResize::getCursorShape(const QPoint &pos) const
{
    const int borderSize = borderLength;

    if (pos.x() <= borderSize && pos.y() <= borderSize)
        return Qt::SizeFDiagCursor;  // Верхний левый угол
    else if (pos.x() <= borderSize && pos.y() >= mainWindow->height() - borderSize)
        return Qt::SizeBDiagCursor;  // Левый нижний угол
    else if (pos.x() <= borderSize)
        return Qt::SizeHorCursor;    // Левая граница
    else if (pos.x() >= mainWindow->width() - borderSize && pos.y() <= borderSize)
        return Qt::SizeBDiagCursor;  // Верхний правый угол
    else if (pos.x() >= mainWindow->width() - borderSize && pos.y() >= mainWindow->height() - borderSize)
        return Qt::SizeFDiagCursor;  // Правый нижний угол
    else if (pos.x() >= mainWindow->width() - borderSize)
        return Qt::SizeHorCursor;    // Правая граница
    else if (pos.y() <= borderSize)
        return Qt::SizeVerCursor;    // Верхняя граница
    else if (pos.y() >= mainWindow->height() - borderSize)
        return Qt::SizeVerCursor;    // Нижняя граница

    return Qt::ArrowCursor;
}

