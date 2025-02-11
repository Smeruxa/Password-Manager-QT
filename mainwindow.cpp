#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    new SettingResize(this);
    opacityHook = new OpacityHook(this);

    iFont = new szFont();

    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralwidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->setAlignment(Qt::AlignTop);

    {
        titleBar = new TitleBar();
        maximizeButton = titleBar->getMaximizeButton();
        mainLayout->addWidget(titleBar->getMainFrame());

        {

            connect(titleBar, &TitleBar::hideMainWindow, this, [this] {
                opacityHook->startAnimation(1.0, 0.0);
                opacityHook->isHiding = true;
            });

            connect(opacityHook, &OpacityHook::animationFinished, this, [this] {
                this->showMinimized();
            });

        }

        connect(titleBar, &TitleBar::restoreMainWindow, this, [this] {
            if (this->isMaximized()) {
                this->showNormal();
                maximizeButton->setText(QChar(0xf2d0));
            }
            else {
                this->showMaximized();
                maximizeButton->setText(QChar(0xf2d2));
            }
        });

        connect(titleBar, &TitleBar::closeMainWindow, this, [this] { this->close(); });
    }

    {
        QFrame* mainContainer = new QFrame();
        mainContainer->setStyleSheet("QFrame { background-color: #102434; }"); // 102434 1f2936
        mainContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QHBoxLayout* mainContainerLayout = new QHBoxLayout();
        mainContainerLayout->setContentsMargins(5, 5, 5, 5);
        mainContainerLayout->setSpacing(0);

        QString framesStyle = "QFrame {"
                              "background-color: #102434;"
                              "color: #899999;"
                              "}";

        {
            QString scrollAreaStyle = "* {"
                                      "    background-color: #102434;"
                                      "}"
                                      "QScrollArea {"
                                      "    border-radius: 5px;"
                                      "}"

                                      "QScrollBar:vertical {"
                                      "    border: 1px solid #a3a3a3;"
                                      "    background-color: #17212b;"
                                      "    width: 13px;"
                                      "    margin: 0px;"
                                      "    margin-top: 5px;"
                                      "    margin-bottom: 5px;"
                                      "    margin-right: 4px;"
                                      "}"

                                      "QScrollBar::handle:vertical {"
                                      "    background-color: #899999;"
                                      "    width: 10px;"
                                      "    min-height: 20px;"
                                      "}"

                                      "QScrollBar::add-line:vertical {"
                                      "    height: 0px;"
                                      "    subcontrol-position: bottom;"
                                      "    subcontrol-origin: margin;"
                                      "}"

                                      "QScrollBar::sub-line:vertical {"
                                      "    height: 0px;"
                                      "    subcontrol-position: top;"
                                      "    subcontrol-origin: margin;"
                                      "}"

                                      "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                                      "    background-color: none;"
                                      "}";

            QVBoxLayout* selectFrameLayout;
            QVBoxLayout* infoFrameLayout;

            QLabel* numberSelected;

            { // Пароли/заметки/логин
                QFrame* mInfoContainer = new QFrame();
                mInfoContainer->setMinimumWidth(400);
                QVBoxLayout* mInfoContainerLayout = new QVBoxLayout();
                mInfoContainerLayout->setContentsMargins(0, 0, 0, 0);
                mInfoContainerLayout->setSpacing(5);
                mInfoContainer->setLayout(mInfoContainerLayout);

                {
                    QFrame* infoFrame = new QFrame();
                    infoFrameLayout = new QVBoxLayout();
                    infoFrameLayout->setContentsMargins(2, 3, 8, 3);
                    infoFrameLayout->setSpacing(15);
                    infoFrameLayout->setAlignment(Qt::AlignTop);
                    infoFrame->setStyleSheet(framesStyle);
                    infoFrame->setLayout(infoFrameLayout);

                    QScrollArea *scrollArea = new QScrollArea();
                    scrollArea->setWidget(infoFrame);
                    scrollArea->setWidgetResizable(true);
                    scrollArea->setContentsMargins(0, 0, 0, 0);
                    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    scrollArea->setStyleSheet(scrollAreaStyle);

                    mInfoContainerLayout->addWidget(scrollArea);
                }

                {
                    QFrame *separator = new QFrame();
                    separator->setFrameShape(QFrame::HLine);
                    separator->setStyleSheet("QFrame { background-color: #899999; border: none; }");
                    separator->setLineWidth(1);
                    separator->setMidLineWidth(0);
                    mInfoContainerLayout->addWidget(separator);
                }

                {
                    numberSelected = new QLabel("Виджет №");
                    numberSelected->setAlignment(Qt::AlignHCenter);
                    numberSelected->setStyleSheet("QLabel { color: #899999; margin-top: 5px; margin-bottom: 5px; }");
                    numberSelected->setFont(iFont->resize(iFont->roboto(), 16));

                    mInfoContainerLayout->addWidget(numberSelected);
                }
                mainContainerLayout->addWidget(mInfoContainer);
            }

            {
                QFrame *separator = new QFrame();
                separator->setFrameShape(QFrame::VLine);
                separator->setStyleSheet("QFrame { background-color: #899999; border: none; }");
                separator->setLineWidth(1);
                separator->setMidLineWidth(0);
                mainContainerLayout->addWidget(separator);
            }

            { // Добавить/удалить
                QFrame* selectFrame = new QFrame();
                selectFrameLayout = new QVBoxLayout();
                selectFrameLayout->setContentsMargins(10, 3, 7, 3);
                selectFrameLayout->setSpacing(5);
                selectFrameLayout->setAlignment(Qt::AlignTop);
                selectFrame->setStyleSheet(framesStyle);
                selectFrame->setLayout(selectFrameLayout);

                QScrollArea *scrollArea = new QScrollArea();
                scrollArea->setMinimumWidth(350);
                scrollArea->setWidget(selectFrame);
                scrollArea->setWidgetResizable(true);
                scrollArea->setContentsMargins(0, 0, 0, 0);
                scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                scrollArea->setStyleSheet(scrollAreaStyle);

                mainContainerLayout->addWidget(scrollArea);
            }

            new Managment(selectFrameLayout, infoFrameLayout, numberSelected);
        }

        mainContainer->setLayout(mainContainerLayout);
        mainLayout->addWidget(mainContainer);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
