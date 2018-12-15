#include <qfile.h>
#define TSF_IMPLEMENTATION
#include "LoadTsf.h"
#include "Utils/QtUtils.h"

tsf* LoadTsf() {
    static QByteArray byteArray = QtUtils::readAllFromFile(":qml/sounds/sounds.sf2");
    return tsf_load_memory(byteArray.data(), byteArray.size());
}


