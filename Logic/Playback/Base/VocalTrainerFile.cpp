#include "VocalTrainerFile.h"
#include "VxFile.h"
#include "MvxFile.h"

VocalTrainerFile* VocalTrainerFile::read(std::istream& is) {
    char vxSignature[2];
    is.read(vxSignature, 2);

    // Check id the file is vx file
    if (vxSignature[0] == 'V' && vxSignature[1] == 'X') {
        is.seekg(0);
        auto* file = new VxFile();
        file->readFromStream(is);
        return file;
    } else {
        // MvxFile
        is.seekg(0);
        auto* file  = new MvxFile();
        file->readFromStream(is);
        return file;
    }
}