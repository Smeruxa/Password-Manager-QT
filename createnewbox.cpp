#include "CreateNewBox.h"

CreateNewBox::CreateNewBox(QLabel *parent, ConfigManager* sConfig, ObjectSelect* sSelectBox)
    : addButton(parent), config(sConfig), selectBox(sSelectBox) {
    AnimationManager* animationManager = new AnimationManager();
    iFont = new szFont();

    animationManager->addAnimation(addButton,
                                   animationManager->hexStringToQColor("#152f43"),
                                   animationManager->hexStringToQColor("#1a3952"));

    addButton->installEventFilter(this);

    {
        mPopup = new QDialog(addButton, Qt::Sheet);
        mPopup->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        mPopup->setFixedWidth(480);
        mPopup->setFixedHeight(250);
        mPopup->setStyleSheet("QDialog { border: 1px solid #899999; color: #899999; background-color: #1f2936; }");

        mPopupLayout = new QHBoxLayout();
        mPopupLayout->setContentsMargins(2, 2, 2, 2);
        mPopupLayout->setSpacing(0);

        {
            {
                QFrame* leftContainer = new QFrame();
                leftContainer->setStyleSheet("QFrame { color: #899999; background-color: #1f2936; }");
                leftContainer->setFixedWidth(210);

                QVBoxLayout* leftContainerLayout = new QVBoxLayout();
                leftContainerLayout->setContentsMargins(0, 0, 0, 0);
                leftContainerLayout->setSpacing(0);
                leftContainerLayout->setAlignment(Qt::AlignCenter);

                leftContainer->setLayout(leftContainerLayout);

                {
                    QLabel* characterIllusion = new QLabel(QChar(0xf505));
                    characterIllusion->setFont(iFont->resize(iFont->regular(), 95));
                    characterIllusion->setStyleSheet("QLabel { border: none; color: #899999; }");

                    QLabel* aboutInfo = new QLabel("Создайте новый виджет!");
                    aboutInfo->setFont(iFont->resize(iFont->roboto(), 11));
                    aboutInfo->setStyleSheet("QLabel { border: none; color: #899999; padding-left: 12px; }");

                    leftContainerLayout->addWidget(characterIllusion);
                    leftContainerLayout->addWidget(aboutInfo);
                }

                mPopupLayout->addWidget(leftContainer);
            }

            {
                QFrame* rightContainer = new QFrame();
                rightContainer->setStyleSheet("QFrame { color: #899999; background-color: #1f2936; }");
                QVBoxLayout* rightContainerLayout = new QVBoxLayout();
                rightContainerLayout->setContentsMargins(0, 0, 0, 0);
                rightContainerLayout->setSpacing(5);
                rightContainerLayout->setAlignment(Qt::AlignCenter);

                rightContainer->setLayout(rightContainerLayout);

                {
                    QString radioButtonStyle =
                        "QRadioButton {"
                        "    color: #899999;"  // Цвет текста
                        "    font-size: 16px;"  // Размер шрифта
                        "    spacing: 7px;"     // Расстояние между текстом и круглым индикатором
                        "    background-color: #1f2936;"
                        "}"
                        "QRadioButton::indicator:hover {"
                        "    background-color: #697d7d;"
                        "}"
                        "QRadioButton::indicator {"
                        "    width: 20px;"      // Ширина круглого индикатора
                        "    height: 20px;"     // Высота круглого индикатора
                        "    border-radius: 10px;"  // Радиус скругления для создания круглого индикатора
                        "    border: 2px solid #899999;"  // Установите стиль границы
                        "}"
                        "QRadioButton::indicator:checked {"
                        "    background-color: #899999;"  // Цвет фона при выборе
                        "    border: 1px solid #899999;"   // Обводка при выборе
                        "}";

                    QRadioButton *password = new QRadioButton("Пароль");
                    password->setStyleSheet(radioButtonStyle);
                    password->setFont(iFont->roboto());
                    QRadioButton *note = new QRadioButton("Заметка");
                    note->setStyleSheet(radioButtonStyle);
                    note->setFont(iFont->roboto());
                    QRadioButton *date = new QRadioButton("Важная дата");
                    date->setStyleSheet(radioButtonStyle);
                    date->setFont(iFont->roboto());

                    {
                        QButtonGroup *buttonGroup = new QButtonGroup();
                        buttonGroup->addButton(password);
                        buttonGroup->addButton(note);
                        buttonGroup->addButton(date);

                        connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [&](QAbstractButton *button) {
                            QRadioButton *selectedRadioButton = dynamic_cast<QRadioButton *>(button);
                            if (selectedRadioButton) {
                                QString selected = selectedRadioButton->text();
                                if (selected == "Пароль") currentChoosed = 0;
                                else if (selected == "Заметка") currentChoosed = 1;
                                else if (selected == "Важная дата") currentChoosed = 2;
                            }
                        });
                    }

                    QFrame* buttonCollector = new QFrame();
                    QHBoxLayout* buttonCollectorLayout = new QHBoxLayout();
                    buttonCollectorLayout->setContentsMargins(0, 0, 0, 0);
                    buttonCollectorLayout->setSpacing(3);

                    {
                        QString pushButtonStyle =
                            "QPushButton {"
                            "    background-color: #2f3a47;"  // Цвет фона
                            "    color: #899999;"  // Цвет текста
                            "    border: 1px solid #2f3a47;"  // Граница
                            "    border-radius: 5px;"  // Скругление углов
                            "    padding: 5px 10px;"  // Отступы внутри кнопки
                            "}"
                            "QPushButton:hover {"
                            "    background-color: #6b757c;"  // Цвет фона при наведении
                            "    border: 1px solid #6b757c;"  // Граница при наведении
                            "}"
                            "QPushButton:pressed {"
                            "    background-color: #48555e;"  // Цвет фона при нажатии
                            "    border: 1px solid #48555e;"  // Граница при нажатии
                            "}";

                        QPushButton* confirmChoose = new QPushButton("Выбрать");
                        confirmChoose->setStyleSheet(pushButtonStyle);
                        confirmChoose->setFont(iFont->roboto());
                        QPushButton* cancelChoose = new QPushButton("Назад");
                        cancelChoose->setStyleSheet(pushButtonStyle);
                        cancelChoose->setFont(iFont->roboto());

                        connect(cancelChoose, &QPushButton::clicked, this, [this] {
                            mPopup->close();
                        });

                        connect(confirmChoose, &QPushButton::clicked, this, [this] {
                            if (currentChoosed != -1) {
                                mPopup->close();

                                int currentDateInt = QDate::currentDate().toJulianDay();

                                switch (currentChoosed) {
                                case 0: {

                                    /*PASSWORD(const QString& name, const QString& site, const QString& login, const QString& password,
                                        const QString& additionalInfo, int data)*/
                                    STRUCTURES::PASSWORD newPassword { "Новый пароль " + QString::number(config->size()),
                                            "Сайт",
                                            "Логин",
                                            "Пароль",
                                            "Дополнительная информация", currentDateInt };

                                    config->addStruct(newPassword);
                                    break;
                                }
                                case 1: { // note
                                    /*NOTE(const QString& name, const QString& note, const QString& additionalInfo, int data)*/
                                    STRUCTURES::NOTE newNote { "Новая заметка " + QString::number(config->size()),
                                            "Заметка",
                                            "Дополнительная информация", currentDateInt };

                                    config->addStruct(newNote);
                                    break;
                                }
                                case 2: { // data
                                    /*DATA(const QString& name, const QString& additionalInfo, int dataActivity, int data)*/
                                    STRUCTURES::DATA newData { "Новая дата " + QString::number(config->size()),
                                            "Дополнительная информация", 0, currentDateInt };

                                    config->addStruct(newData);
                                    break;
                                }
                                default:
                                    break;
                                }

                                selectBox->refillLayout();
                            }
                        });

                        buttonCollectorLayout->addWidget(confirmChoose);
                        buttonCollectorLayout->addWidget(cancelChoose);
                    }

                    buttonCollector->setLayout(buttonCollectorLayout);

                    rightContainerLayout->addWidget(password);
                    rightContainerLayout->addWidget(note);
                    rightContainerLayout->addWidget(date);

                    rightContainerLayout->addWidget(buttonCollector);
                }

                mPopupLayout->addWidget(rightContainer);
            }
        }

        mPopup->setLayout(mPopupLayout);
    }
}

bool CreateNewBox::eventFilter(QObject *obj, QEvent *event) {
    if (obj == addButton) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

            if (addButton->rect().contains(mouseEvent->pos())) {
                mPopup->exec();
            }
        }
    }

    return QObject::eventFilter(obj, event);
}
