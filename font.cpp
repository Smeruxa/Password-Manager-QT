#include "font.h"

szFont::szFont()
{
    initializate();
}

QFont szFont::resize(QFont _font, int pixels) {
    return QFont(_font.family(), pixels, _font.weight());
}

void szFont::initializate() {
    if (QFontDatabase::addApplicationFont(":/resources/font/Roboto/Roboto-Bold.ttf") < 0)
        qWarning() << "Roboto не найден.";

    if (QFontDatabase::addApplicationFont(":/resources/font/Font Awesome 6 Free-Solid-900.otf") < 0)
        qWarning() << "FontAwesome Solid не найден.";

    if (QFontDatabase::addApplicationFont(":/resources/font/Font Awesome 6 Free-Regular-400.otf") < 0)
        qWarning() << "FontAwesome Regular не найден.";

    faSolid = QFont("FontAwesome", 11, QFont::Bold);
    faRegular = QFont("FontAwesome", 11, QFont::Normal);
    fontRoboto = QFont("Roboto", 11, QFont::Normal);
}
