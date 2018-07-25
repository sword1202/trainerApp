#include "fonts.h"
#include "QtUtils/qtutils.h"

namespace Fonts
{
    QByteArray latoRegular() {
        static QByteArray data = QtUtils::readAllFromFile(":qml/fonts/Lato-Regular.ttf");
        return data;
    }

    QByteArray latoBold() {
        static QByteArray data = QtUtils::readAllFromFile(":qml/fonts/Lato-Bold.ttf");
        return data;
    }
}