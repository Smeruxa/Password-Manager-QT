#include "managment.h"

Managment::Managment(QVBoxLayout *selectLayout, QVBoxLayout *infoLayout, QLabel *selectedNumber) : selectLayout(selectLayout), infoLayout(infoLayout), selectedNumber(selectedNumber)
{
    iFont = new szFont();
    ConfigManager* config = new ConfigManager();

    QString framesStyle = "QFrame {"
                          "background-color: #102434;"
                          "color: #899999;"
                          "}";

    {
        QFrame* textFrame = new QFrame();
        textFrameLayout = new QHBoxLayout();
        textFrameLayout->setAlignment(Qt::AlignCenter);
        textFrameLayout->setSpacing(2);
        textFrameLayout->setContentsMargins(0, 0, 0, 0);

        QLabel* exampleText = new QLabel("Новый виджет");
        exampleText->setStyleSheet("QLabel { color: #899999; }");
        exampleText->setFont(iFont->resize(iFont->roboto(), 16));

        QLabel* exampleData = new QLabel("dd.mm.yy");
        exampleData->setStyleSheet("QLabel { color: #899999; }");
        exampleData->setFont(iFont->resize(iFont->roboto(), 10));
        exampleData->setAlignment(Qt::AlignBottom);

        textFrameLayout->addWidget(exampleText);
        textFrameLayout->addWidget(exampleData);

        textFrame->setLayout(textFrameLayout);
        infoLayout->addWidget(textFrame);

        for (int i = 0; i < 5; i++) {
            QFrame* exampleFrame = new QFrame();
            exampleFrame->setFixedHeight(i != 4 ? 40 : 235);
            exampleFrame->setStyleSheet("QFrame {"
                                        "background-color: #152f43;"
                                        "}");

            infoLayout->addWidget(exampleFrame);
        }
    }

    {
        QFrame *additionalSelect = new QFrame();
        additionalSelect->setStyleSheet(framesStyle);
        additionalSelect->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QVBoxLayout *additionalSelectLayout = new QVBoxLayout();
        additionalSelectLayout->setContentsMargins(0, 0, 0, 0);
        additionalSelectLayout->setSpacing(3);

        {
            selectBox = new ObjectSelect(additionalSelectLayout, config);
            selectBox->refillLayout();

            connect(selectBox, &ObjectSelect::defaultLayout, this, [this, infoLayout]() {
                currentIndex = -1;

                QLayoutItem *item;
                while ((item = infoLayout->takeAt(0)) != nullptr) {
                    QWidget *widget = item->widget();
                    if (widget) {
                        infoLayout->removeWidget(widget);
                        delete widget;
                    } else {
                        delete item;
                    }
                }

                QFrame* textFrame = new QFrame();
                textFrameLayout = new QHBoxLayout();
                textFrameLayout->setAlignment(Qt::AlignCenter);
                textFrameLayout->setSpacing(2);
                textFrameLayout->setContentsMargins(0, 0, 0, 0);

                QLabel* exampleText = new QLabel("Новый виджет");
                exampleText->setStyleSheet("QLabel { color: #899999; }");
                exampleText->setFont(iFont->resize(iFont->roboto(), 16));

                QLabel* exampleData = new QLabel("dd.mm.yy");
                exampleData->setStyleSheet("QLabel { color: #899999; }");
                exampleData->setFont(iFont->resize(iFont->roboto(), 10));
                exampleData->setAlignment(Qt::AlignBottom);

                textFrameLayout->addWidget(exampleText);
                textFrameLayout->addWidget(exampleData);

                textFrame->setLayout(textFrameLayout);
                infoLayout->addWidget(textFrame);

                for (int i = 0; i < 5; i++) {
                    QFrame* exampleFrame = new QFrame();
                    exampleFrame->setFixedHeight(i != 4 ? 40 : 235);
                    exampleFrame->setStyleSheet("QFrame {"
                                                "background-color: #152f43;"
                                                "}");

                    infoLayout->addWidget(exampleFrame);
                }
            });

            ShowEdit* showFrames = new ShowEdit(infoLayout, config, iFont, selectBox);

            connect(selectBox, &ObjectSelect::clickedIndex, this, [this, infoLayout, config, selectedNumber, showFrames](int index) {
                currentIndex = index;

                QLayoutItem *item;
                while ((item = infoLayout->takeAt(0)) != nullptr) {
                    QWidget *widget = item->widget();
                    if (widget) {
                        infoLayout->removeWidget(widget);
                        delete widget;
                    } else {
                        delete item;
                    }
                }

                {
                    STRUCTURES::TYPES type = config->getStructType(index);

                    switch (type) {
                    case STRUCTURES::PASSWORD_TYPE: {
                        showFrames->initPage<STRUCTURES::PASSWORD>(index);
                        break;
                    }
                    case STRUCTURES::NOTE_TYPE: {
                        showFrames->initPage<STRUCTURES::NOTE>(index);
                        break;
                    }
                    case STRUCTURES::DATA_TYPE: {
                        showFrames->initPage<STRUCTURES::DATA>(index);
                        break;
                    }
                    default:
                        break;
                    }

                    selectedNumber->setText("Виджет №" + QString::number(index));
                }
            });
        }

        additionalSelect->setLayout(additionalSelectLayout);
        selectLayout->addWidget(additionalSelect);
    }

    {
        QFrame *buttonFrame = new QFrame();
        buttonFrame->setFixedHeight(50);
        QVBoxLayout *buttonFrameLayout = new QVBoxLayout();
        buttonFrameLayout->setContentsMargins(0, 0, 0, 0);
        buttonFrameLayout->setSpacing(0);
        buttonFrameLayout->setAlignment(Qt::AlignCenter);

        QLabel *addButton = new QLabel(QChar(0xf0fe));
        addButton->setAlignment(Qt::AlignCenter);
        addButton->setFixedWidth(120);
        addButton->setStyleSheet("QLabel { border: 1px solid #899999; padding-top: 2px; background-color: #152f43; border-radius: 25% 10%; }");
        addButton->setFont(iFont->resize(iFont->regular(), 33));
        buttonFrameLayout->addWidget(addButton);

        new CreateNewBox(addButton, config, selectBox);

        buttonFrame->setLayout(buttonFrameLayout);
        selectLayout->addWidget(buttonFrame);
    }
}

/*
 *                                 QTextEdit* loginText = new QTextEdit();
                                loginText->document()->setDefaultStyleSheet("color: #899999;");
                                loginText->setMinimumHeight(40);
                                loginText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

                                loginText->setStyleSheet("QTextEdit { background-color: #152f43; border: none; }"
                                                         "QMenu { background-color: #1c2d48; }"
                                                         "QMenu::item { color: #899999; font: 13px \"Roboto\", sans-serif; margin-left: 3px; margin-top: 3px; margin-bottom: 3px; }"
                                                         "QMenu::item:selected { background-color: #233759; color: white; }");

                                connect(loginText, &QTextEdit::textChanged, this, [loginText] {
                                    loginText->setFixedHeight(loginText->document()->size().toSize().height());
                                });

                                loginText->setFont(iFont->resize(iFont->roboto(), 15));
                                loginText->setText(loadedPassword.login + "dfsouihgilsudgiuoscvbuio;xcfvdgbhhuiodvbhuiodvbioudvhbuidshbdfgjudsfgsddgsdfgsdfgsdfgsdfgsdgfsdfgsdfgsdfgsfgsdfgsdfgsdfgsdfgsdfgsg");
                                loginText->setFixedHeight(60);
 * */
