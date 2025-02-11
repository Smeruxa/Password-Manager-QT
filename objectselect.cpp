#include "objectselect.h"

ObjectSelect::ObjectSelect(QVBoxLayout* parentLayout, ConfigManager* config, QVBoxLayout* infoLayout) : parentLayout(parentLayout), config(config), infoLayout(infoLayout) {
    iFont = new szFont();
}

template<typename T>
void ObjectSelect::addBox(T& structure, QChar symbolType) {
    createBox(structure.name, structure.additionalInfo, symbolType);
}

void ObjectSelect::refillLayout() {
    currentIndex = 0;
    deleteIndex = 0;

    QLayoutItem *item;
    while ((item = parentLayout->takeAt(0)) != nullptr) {
        QWidget *widget = item->widget();
        if (widget) {
            parentLayout->removeWidget(widget);
            delete widget;
        } else {
            delete item;
        }
    }

    for (int i = 0; i < config->size(); i++) {
        STRUCTURES::TYPES type = config->getStructType(i);

        switch (type) {
        case STRUCTURES::PASSWORD_TYPE: {
            STRUCTURES::PASSWORD loadedPassword = config->getStruct<STRUCTURES::PASSWORD>(i);
            addBox(loadedPassword, QChar(0xf13e));
            break;
        }
        case STRUCTURES::NOTE_TYPE: {
            STRUCTURES::NOTE loadedNote = config->getStruct<STRUCTURES::NOTE>(i);
            addBox(loadedNote, QChar(0xf086));
            break;
        }
        case STRUCTURES::DATA_TYPE: {
            STRUCTURES::DATA loadedData = config->getStruct<STRUCTURES::DATA>(i);
            addBox(loadedData, QChar(0xf784));
            break;
        }
        default:
            break;
        }
    }
}

void ObjectSelect::createBox(QString mainText, QString additionalText, QChar symbolType) {
    QFrame *mainBox = new QFrame();
    mainBox->setFixedHeight(50);
    mainBox->setStyleSheet("QFrame {"
                           "background-color: #212e41;"
                           "color: #899999;"
                           "border: 1px solid #899999;"
                           "}");

    mainBox->setObjectName(QString("mainBox_%1").arg(currentIndex));
    mainBox->installEventFilter(this);

    currentIndex++;

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    {
        QString casesStyle = "QLabel, QFrame { border: none }";

        QFrame* textBox = new QFrame();
        textBox->setStyleSheet(casesStyle);
        QVBoxLayout* textLayout = new QVBoxLayout();
        textLayout->setAlignment(Qt::AlignLeft);
        textLayout->setContentsMargins(5, 2, 5, 2);
        textLayout->setSpacing(1);
        textBox->setLayout(textLayout);
        {
            {
                QFrame* upCase = new QFrame();
                QHBoxLayout* upCaseLayout = new QHBoxLayout();
                upCaseLayout->setAlignment(Qt::AlignLeft);
                upCaseLayout->setContentsMargins(1, 0, 0, 0);
                upCaseLayout->setSpacing(7);

                {
                    QLabel* upCaseSymbol = new QLabel(symbolType);
                    upCaseSymbol->setStyleSheet(casesStyle);
                    upCaseSymbol->setFont(iFont->resize(iFont->solid(), (additionalText.length() > 0 ? 15 : 20)));
                    upCaseLayout->addWidget(upCaseSymbol);

                    QLabel* upCaseText = new QLabel(shortString(mainText, 23));
                    upCaseText->setStyleSheet(casesStyle);
                    upCaseText->setFont(iFont->resize(iFont->roboto(), (additionalText.length() > 0 ? 15 : 20)));
                    upCaseLayout->addWidget(upCaseText);
                }

                upCase->setLayout(upCaseLayout);

                textLayout->addWidget(upCase);

                if (additionalText.length() > 0) {
                    QLabel* underCase = new QLabel(shortString(additionalText, 35));
                    underCase->setStyleSheet(casesStyle);
                    underCase->setFont(iFont->resize(iFont->roboto(), 11));
                    textLayout->addWidget(underCase);
                }
            }
        }
        mainLayout->addWidget(textBox);
    }

    mainLayout->addStretch();

    {
        QFrame* deleteFrame = new QFrame();
        deleteFrame->setStyleSheet("QFrame { border: none; }");
        deleteFrame->setObjectName("STOP_FRAME");

        QHBoxLayout* deleteFrameLayout = new QHBoxLayout(deleteFrame);

        QPushButton* deleteButton = new QPushButton();
        deleteButton->setFont(iFont->resize(iFont->solid(), 14));
        deleteButton->setText(QChar(0xf1f8));
        deleteButton->setFixedWidth(35);
        deleteButton->setStyleSheet("QPushButton { "
                                    "color: #899999;"
                                    "background-color: #212e41;"
                                    "border: none;"
                                    "}");

        deleteButton->setObjectName(QString("deleteButton_%1").arg(deleteIndex));

        connect(deleteButton, &QPushButton::clicked, this, [this, deleteButton]() {
            bool ok;
            int index = deleteButton->objectName().mid(QString("deleteButton_").length()).toInt(&ok);
            if (ok) {
                config->removeStruct(index);
                refillLayout();

                emit defaultLayout();
            }
        });

        deleteIndex++;

        deleteFrameLayout->addWidget(deleteButton);

        mainLayout->addWidget(deleteFrame);
    }

    mainBox->setLayout(mainLayout);

    parentLayout->addWidget(mainBox);
}

bool ObjectSelect::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QFrame *clickedFrame = qobject_cast<QFrame*>(obj);
        if (clickedFrame && clickedFrame->objectName().indexOf("STOP_FRAME") == -1) {
            bool ok;
            int index = clickedFrame->objectName().mid(QString("mainBox_").length()).toInt(&ok);
            if (ok)
                emit clickedIndex(index);
        }
    }

    // Передаем событие дальше
    return QObject::eventFilter(obj, event);
}

QString ObjectSelect::shortString(QString inputString, int maxLength) {
    if (inputString.length() > maxLength)
        return inputString.left(maxLength - 3) + "...";
    else
        return inputString;
}

template void ObjectSelect::addBox<STRUCTURES::PASSWORD>(STRUCTURES::PASSWORD&, QChar);
template void ObjectSelect::addBox<STRUCTURES::NOTE>(STRUCTURES::NOTE&, QChar);
template void ObjectSelect::addBox<STRUCTURES::DATA>(STRUCTURES::DATA&, QChar);
