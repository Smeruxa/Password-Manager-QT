#include "titlebar.h"

TitleBar::TitleBar()
{
    iFont = new szFont();

    mainFrame = new QFrame();
    mainFrame->setStyleSheet("QFrame {"
                             "background-color: #1d2e40;"
                             "color: #919191;"
                             "}");

    animationManager = new AnimationManager();

    {

        QHBoxLayout *layout = new QHBoxLayout(mainFrame);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        {
            QLabel* titleBar = new QLabel("Password Manager");
            titleBar->setFont(iFont->roboto());
            titleBar->setStyleSheet("QLabel { color: #899999; margin-left: 5px; }");
            titleBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

            layout->addWidget(titleBar);

            QPushButton* buttons[] = { hideButton, maximizeButton, closeButton };
            QChar buttonIcons[] = { QChar(0xf7a4), QChar(0xf2d0), QChar(0xf00d) };
            std::function<void()> buttonCallbacks[] = {
                [this] { emit hideMainWindow(); },
                [this] { emit restoreMainWindow(); },
                [this] { emit closeMainWindow(); }
            };

            for (int i = 0; i < 3; i++) {
                buttons[i] = new QPushButton(buttonIcons[i]);

                buttons[i]->setFont(i == 2 ? iFont->solid() : iFont->regular());
                buttons[i]->setStyleSheet("QPushButton { color: #899999; width: 40px; height: 25px; background-color: #1d2e40; border: none; }");
                animationManager->addAnimation(buttons[i],
                                               animationManager->hexStringToQColor("#1d2e40"),
                                               i == 2 ? animationManager->hexStringToQColor("#c72844") : animationManager->hexStringToQColor("#2e4157")
                                               );

                layout->addWidget(buttons[i]);

                if (i == 1)
                    maximizeButton = buttons[i];

                connect(buttons[i], &QPushButton::clicked, this, buttonCallbacks[i]);
            }
        }
    }

    {
        mainFrame->setFixedHeight(0);

        QPropertyAnimation *animation = new QPropertyAnimation(mainFrame, "maximumHeight");
        animation->setDuration(400);
        animation->setStartValue(0);
        animation->setEndValue(25);

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    mainFrame->installEventFilter(this);
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == mainFrame)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            startDrag(static_cast<QMouseEvent*>(event));
            return true;
        }
        else if (event->type() == QEvent::MouseMove)
        {
            dragMove(static_cast<QMouseEvent*>(event));
            return true;
        }
    }

    return QObject::eventFilter(obj, event);
}

void TitleBar::startDrag(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->pos();
        event->accept();
    }
}

QPushButton* TitleBar::getMaximizeButton() { return maximizeButton; }

void TitleBar::dragMove(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QMainWindow *mainWindow = qobject_cast<QMainWindow*>(qApp->activeWindow());

        if (mainWindow)
        {
            mainWindow->move(event->globalPosition().toPoint() - dragPosition);
            event->accept();
        }
    }
}

QFrame* TitleBar::getMainFrame() const
{
    return mainFrame;
}
