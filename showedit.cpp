#include "showedit.h"

ShowEdit::ShowEdit(QVBoxLayout* layout, ConfigManager* config, szFont* font, ObjectSelect* selectBox)
    : mainLayout(layout), config(config), selectBox(selectBox), iFont(font) {}

template<typename T>
QLineEdit* ShowEdit::mutableString(QHBoxLayout* secondLayout, T structure, int index, QString& field, int fieldIndex, bool isBlue) {
    QPushButton* edit = new QPushButton(QChar(0xf304));
    edit->setFont(iFont->resize(iFont->solid(), 15));
    edit->setMinimumWidth(35);
    edit->setMaximumWidth(35);

    edit->setStyleSheet("QPushButton { background-color: #102434; color: #899999; border: none; }");
    edit->setObjectName("lineEdit_" + QString::number(COUNT_EDITS));
    isEditing["lineEdit_" + QString::number(COUNT_EDITS)] = false;

    QLineEdit* lineEdit = new QLineEdit();
    lineEdit->setReadOnly(true);
    lineEdit->setMinimumWidth(100);
    lineEdit->setFont(iFont->resize(iFont->roboto(), 16));
    lineEdit->setText(field);
    lineEdit->setCursorPosition(0);
    if (!isBlue)
        lineEdit->setStyleSheet("QLineEdit { background-color: #102434; color: #899999; border: none; padding-left: 5px; }"
                            "QMenu { background-color: #1c2d48; }"
                            "QMenu::item { color: #899999; font: 13px \"Roboto\", sans-serif; margin-left: 3px; margin-top: 3px; margin-bottom: 3px; }"
                            "QMenu::item:selected { background-color: #233759; color: white; }");
    else {
        lineEdit->setStyleSheet("QLineEdit { background-color: #152f43; color: #899999; border: none; padding-left: 5px; }"
                                "QMenu { background-color: #1c2d48; }"
                                "QMenu::item { color: #899999; font: 13px \"Roboto\", sans-serif; margin-left: 3px; margin-top: 3px; margin-bottom: 3px; }"
                                "QMenu::item:selected { background-color: #233759; color: white; }");
        lineEdit->setFixedHeight(40);
    }

    connect(edit, &QPushButton::clicked, this, [this, edit, lineEdit, structure, index, field, fieldIndex]() {
        QString numEdit = edit->objectName();

        /*
         * Идея такая:
         * узнавать по полю какое значение структуры нужно изменять
         * а затем это значение изменять прям полностью внутри структуры, создавая новую, с теми же значениями, но при этом одно значение другое будет
         * */

        if (lineEdit->text() != "") {
            if (isEditing[numEdit]) {
                /*
                 *         PASSWORD(const QString& name, const QString& site, const QString& login, const QString& password,
                 const QString& additionalInfo, int data)
                 */

                if constexpr (std::is_same_v<T, STRUCTURES::PASSWORD>) {
                    STRUCTURES::PASSWORD installStructure = structure;

                    switch (fieldIndex) {
                    case 1:
                        installStructure.name = lineEdit->text();
                        break;
                    case 2:
                        installStructure.site = lineEdit->text();
                        break;
                    case 3:
                        installStructure.login = lineEdit->text();
                        break;
                    case 4:
                        installStructure.password = lineEdit->text();
                        break;
                    default:
                        qWarning() << "Install was broken | Password";
                        break;
                    }

                    config->setStructure(index, installStructure);
                } else if constexpr (std::is_same_v<T, STRUCTURES::NOTE>) {
                    /*
                     *         NOTE(const QString& name, const QString& note, const QString& additionalInfo, int data)
            : note(note), data(data) {
                     * */

                    STRUCTURES::NOTE installStructure = structure;

                    switch (fieldIndex) {
                    case 1:
                        installStructure.name = lineEdit->text();
                        break;
                    default:
                        qWarning() << "Install was broken | Note";
                        break;
                    }

                    config->setStructure(index, installStructure);
                } else if constexpr (std::is_same_v<T, STRUCTURES::DATA>) {
                    /*
                     *         DATA(const QString& name, const QString& additionalInfo, int dataActivity, int data)
            : dataActivity(dataActivity), data(data) {
                     * */
                    STRUCTURES::DATA installStructure = structure;

                    switch (fieldIndex) {
                    case 1:
                        installStructure.name = lineEdit->text();
                        break;
                    default:
                        qWarning() << "Install was broken | Data";
                        break;
                    }

                    config->setStructure(index, installStructure);
                }
            }

            selectBox->refillLayout();
            lineEdit->setCursorPosition(0);

            isEditing[numEdit] = !isEditing[numEdit];

            edit->setText(isEditing[numEdit] ? QChar(0xf00c) : QChar(0xf304));
            lineEdit->setReadOnly(!isEditing[numEdit]);
        }
    });

    secondLayout->addWidget(edit);
    secondLayout->addWidget(lineEdit);

    COUNT_EDITS += 1;
    return lineEdit;
}

template<typename T>
QTextEdit* ShowEdit::mutableMultiline(QHBoxLayout* secondLayout, T structure, int index, QString& field, int fieldIndex) {
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
    QPushButton* edit = new QPushButton(QChar(0xf304));
    edit->setFont(iFont->resize(iFont->solid(), 15));
    edit->setMinimumWidth(35);
    edit->setMaximumWidth(35);

    edit->setStyleSheet("QPushButton { background-color: #102434; color: #899999; border: none; }");
    edit->setObjectName("multilineEdit_" + QString::number(COUNT_MULTILINE_EDITS));
    isEditing["multilineEdit_" + QString::number(COUNT_MULTILINE_EDITS)] = false;

    QTextEdit* multilineEdit = new QTextEdit();
    multilineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    multilineEdit->setReadOnly(true);
    multilineEdit->setMinimumWidth(100);
    multilineEdit->setMinimumHeight(40);
    multilineEdit->setFont(iFont->resize(iFont->roboto(), 15));
    multilineEdit->setPlainText(field);
    multilineEdit->setFixedHeight(120);
    multilineEdit->setStyleSheet("QTextEdit { background-color: #152f43; color: #899999; border: none; padding-left: 5px; }"
                                "QMenu { background-color: #1c2d48; }"
                                "QMenu::item { color: #899999; font: 13px \"Roboto\", sans-serif; margin-left: 3px; margin-top: 3px; margin-bottom: 3px; }"
                                "QMenu::item:selected { background-color: #233759; color: white; }");

    connect(edit, &QPushButton::clicked, this, [this, edit, multilineEdit, structure, index, field, fieldIndex]() {
        QString numEdit = edit->objectName();

        /*
         * Идея такая:
         * узнавать по полю какое значение структуры нужно изменять
         * а затем это значение изменять прям полностью внутри структуры, создавая новую, с теми же значениями, но при этом одно значение другое будет
         * */

        if (multilineEdit->toPlainText() != "") {
            if (isEditing[numEdit]) {
                /*
                 *         PASSWORD(const QString& name, const QString& site, const QString& login, const QString& password,
                 const QString& additionalInfo, int data)
                 */

                if constexpr (std::is_same_v<T, STRUCTURES::PASSWORD>) {
                    STRUCTURES::PASSWORD installStructure = structure;

                    if (fieldIndex == 5)
                        installStructure.additionalInfo = multilineEdit->toPlainText();
                    else
                        qWarning() << "Install was broken | Password";

                    config->setStructure(index, installStructure);
                } else if constexpr (std::is_same_v<T, STRUCTURES::NOTE>) {
                    /*
                     *         NOTE(const QString& name, const QString& note, const QString& additionalInfo, int data)
            : note(note), data(data) {
                     * */

                    STRUCTURES::NOTE installStructure = structure;

                    switch (fieldIndex) {
                    case 2:
                        installStructure.note = multilineEdit->toPlainText();
                        break;
                    case 3:
                        installStructure.additionalInfo = multilineEdit->toPlainText();
                        break;
                    default:
                        qWarning() << "Install was broken | Note";
                        break;
                    }

                    config->setStructure(index, installStructure);
                } else if constexpr (std::is_same_v<T, STRUCTURES::DATA>) {
                    /*
                     *         DATA(const QString& name, const QString& additionalInfo, int dataActivity, int data)
            : dataActivity(dataActivity), data(data) {
                     * */
                    STRUCTURES::DATA installStructure = structure;

                    if (fieldIndex == 2)
                        installStructure.additionalInfo = multilineEdit->toPlainText();
                    else
                        qWarning() << "Install was broken | Data";

                    config->setStructure(index, installStructure);
                }
            }

            selectBox->refillLayout();

            isEditing[numEdit] = !isEditing[numEdit];

            edit->setText(isEditing[numEdit] ? QChar(0xf00c) : QChar(0xf304));
            multilineEdit->setReadOnly(!isEditing[numEdit]);
        }
    });

    secondLayout->addWidget(edit);
    secondLayout->addWidget(multilineEdit);

    COUNT_MULTILINE_EDITS += 1;
    return multilineEdit;
}

template<typename T>
void ShowEdit::initPage(int index) {

    if constexpr (std::is_same_v<T, STRUCTURES::PASSWORD>) {
        STRUCTURES::PASSWORD loadedPassword = config->getStruct<STRUCTURES::PASSWORD>(index);

        {
            QFrame* textFrame = new QFrame();
            QHBoxLayout* textFrameLayout = new QHBoxLayout();
            textFrameLayout->setSpacing(2);
            textFrameLayout->setContentsMargins(0, 0, 0, 0);

            QLineEdit* structName = mutableString(textFrameLayout, loadedPassword, index, loadedPassword.name, 1, false);
            structName->setAlignment(Qt::AlignCenter);

            QLabel* data = new QLabel(QDate::fromJulianDay(loadedPassword.data).toString("dd.MM.yy"));
            data->setStyleSheet("QLabel { color: #899999; }");
            data->setFont(iFont->resize(iFont->roboto(), 10));
            data->setAlignment(Qt::AlignBottom);
            data->setMinimumWidth(55);
            data->setMaximumWidth(55);

            textFrameLayout->addWidget(data);

            textFrame->setLayout(textFrameLayout);
            mainLayout->addWidget(textFrame);
        }

        {
            QFrame* siteFrame = new QFrame();
            QHBoxLayout* siteFrameLayout = new QHBoxLayout();
            siteFrameLayout->setSpacing(2);
            siteFrameLayout->setContentsMargins(0, 0, 0, 0);

            QLineEdit* structSite = mutableString(siteFrameLayout, loadedPassword, index, loadedPassword.site, 2, true);
            structSite->setAlignment(Qt::AlignLeft);

            siteFrame->setLayout(siteFrameLayout);
            mainLayout->addWidget(siteFrame);
        }

        {
            QFrame* loginFrame = new QFrame();
            QHBoxLayout* loginFrameLayout = new QHBoxLayout();
            loginFrameLayout->setSpacing(2);
            loginFrameLayout->setContentsMargins(0, 0, 0, 0);

            QLineEdit* structSite = mutableString(loginFrameLayout, loadedPassword, index, loadedPassword.login, 3, true);
            structSite->setAlignment(Qt::AlignLeft);

            loginFrame->setLayout(loginFrameLayout);
            mainLayout->addWidget(loginFrame);
        }

        {
            QFrame* passwordFrame = new QFrame();
            QHBoxLayout* passwordFrameLayout = new QHBoxLayout();
            passwordFrameLayout->setSpacing(2);
            passwordFrameLayout->setContentsMargins(0, 0, 0, 0);

            QLineEdit* structSite = mutableString(passwordFrameLayout, loadedPassword, index, loadedPassword.password, 4, true);
            structSite->setAlignment(Qt::AlignLeft);

            passwordFrame->setLayout(passwordFrameLayout);
            mainLayout->addWidget(passwordFrame);
        }

        {
            QFrame* additionalFrame = new QFrame();
            QHBoxLayout* additionalFrameLayout = new QHBoxLayout();
            additionalFrameLayout->setSpacing(2);
            additionalFrameLayout->setContentsMargins(0, 0, 0, 0);

            QTextEdit* structAdditional = mutableMultiline(additionalFrameLayout, loadedPassword, index, loadedPassword.additionalInfo, 5);
            structAdditional->setAlignment(Qt::AlignLeft);

            additionalFrame->setLayout(additionalFrameLayout);
            mainLayout->addWidget(additionalFrame);
        }

    } else if constexpr (std::is_same_v<T, STRUCTURES::NOTE>) {
        STRUCTURES::NOTE loadedNote = config->getStruct<STRUCTURES::NOTE>(index);

        {
            QFrame* textFrame = new QFrame();
            QHBoxLayout* textFrameLayout = new QHBoxLayout();
            textFrameLayout->setSpacing(2);
            textFrameLayout->setContentsMargins(0, 0, 0, 0);

            QLineEdit* structName = mutableString(textFrameLayout, loadedNote, index, loadedNote.name, 1, false);
            structName->setAlignment(Qt::AlignCenter);

            QLabel* data = new QLabel(QDate::fromJulianDay(loadedNote.data).toString("dd.MM.yy"));
            data->setStyleSheet("QLabel { color: #899999; }");
            data->setFont(iFont->resize(iFont->roboto(), 10));
            data->setAlignment(Qt::AlignBottom);
            data->setMinimumWidth(55);
            data->setMaximumWidth(55);

            textFrameLayout->addWidget(data);

            textFrame->setLayout(textFrameLayout);
            mainLayout->addWidget(textFrame);
        }

        {
            QFrame* noteFrame = new QFrame();
            QHBoxLayout* noteFrameLayout = new QHBoxLayout();
            noteFrameLayout->setSpacing(2);
            noteFrameLayout->setContentsMargins(0, 0, 0, 0);

            QTextEdit* structNote = mutableMultiline(noteFrameLayout, loadedNote, index, loadedNote.note, 2);
            structNote->setAlignment(Qt::AlignLeft);

            noteFrame->setLayout(noteFrameLayout);
            mainLayout->addWidget(noteFrame);
        }

        {
            QFrame* additionalFrame = new QFrame();
            QHBoxLayout* additionalFrameLayout = new QHBoxLayout();
            additionalFrameLayout->setSpacing(2);
            additionalFrameLayout->setContentsMargins(0, 0, 0, 0);

            QTextEdit* structAdditional = mutableMultiline(additionalFrameLayout, loadedNote, index, loadedNote.additionalInfo, 3);
            structAdditional->setAlignment(Qt::AlignLeft);

            additionalFrame->setLayout(additionalFrameLayout);
            mainLayout->addWidget(additionalFrame);
        }

    } else if constexpr (std::is_same_v<T, STRUCTURES::DATA>) {
        STRUCTURES::DATA loadedData = config->getStruct<STRUCTURES::DATA>(index);

        {
            QFrame* textFrame = new QFrame();
            QHBoxLayout* textFrameLayout = new QHBoxLayout();
            textFrameLayout->setSpacing(2);
            textFrameLayout->setContentsMargins(0, 0, 0, 0);

            QLineEdit* structName = mutableString(textFrameLayout, loadedData, index, loadedData.name, 1, false);
            structName->setAlignment(Qt::AlignCenter);

            QLabel* data = new QLabel(QDate::fromJulianDay(loadedData.data).toString("dd.MM.yy"));
            data->setStyleSheet("QLabel { color: #899999; }");
            data->setFont(iFont->resize(iFont->roboto(), 10));
            data->setAlignment(Qt::AlignBottom);
            data->setMinimumWidth(55);
            data->setMaximumWidth(55);

            textFrameLayout->addWidget(data);

            textFrame->setLayout(textFrameLayout);
            mainLayout->addWidget(textFrame);
        }

        {
            QFrame* additionalFrame = new QFrame();
            QHBoxLayout* additionalFrameLayout = new QHBoxLayout();
            additionalFrameLayout->setSpacing(2);
            additionalFrameLayout->setContentsMargins(0, 0, 0, 0);

            QTextEdit* structAdditional = mutableMultiline(additionalFrameLayout, loadedData, index, loadedData.additionalInfo, 2);
            structAdditional->setAlignment(Qt::AlignLeft);

            additionalFrame->setLayout(additionalFrameLayout);
            mainLayout->addWidget(additionalFrame);
        }
    } else {
        qDebug() << "Unknown type to show";
    }
}

template void ShowEdit::initPage<STRUCTURES::PASSWORD>(int);
template void ShowEdit::initPage<STRUCTURES::NOTE>(int);
template void ShowEdit::initPage<STRUCTURES::DATA>(int);

template QLineEdit* ShowEdit::mutableString<typename STRUCTURES::PASSWORD>(QHBoxLayout* secondLayout, STRUCTURES::PASSWORD structure, int index, QString& field, int fieldIndex, bool isBlue);
template QLineEdit* ShowEdit::mutableString<typename STRUCTURES::NOTE>(QHBoxLayout* secondLayout, STRUCTURES::NOTE structure, int index, QString& field, int fieldIndex, bool isBlue);
template QLineEdit* ShowEdit::mutableString<typename STRUCTURES::DATA>(QHBoxLayout* secondLayout, STRUCTURES::DATA structure, int index, QString& field, int fieldIndex, bool isBlue);

template QTextEdit* ShowEdit::mutableMultiline<typename STRUCTURES::PASSWORD>(QHBoxLayout* secondLayout, STRUCTURES::PASSWORD structure, int index, QString& field, int fieldIndex);
template QTextEdit* ShowEdit::mutableMultiline<typename STRUCTURES::NOTE>(QHBoxLayout* secondLayout, STRUCTURES::NOTE structure, int index, QString& field, int fieldIndex);
template QTextEdit* ShowEdit::mutableMultiline<typename STRUCTURES::DATA>(QHBoxLayout* secondLayout, STRUCTURES::DATA structure, int index, QString& field, int fieldIndex);
