#include <qfile.h>
#include "LoadTsf.h"

tsf* LoadTsf() {
    QFile file(":qml/sounds/sounds.sf2");
    file.open(QFile::ReadOnly);
    QByteArray byteArray = file.readAll();
    return tsf_load_memory(byteArray.data(), byteArray.size());
}


