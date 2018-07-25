#include "fonts.h"
#include "QtUtils/qtutils.h"

namespace Fonts
{
    QByteArray arial() {
        static QByteArray data = QtUtils::readAllFromFile(":qml/fonts/arial.ttf");
        return data;
    }
}
