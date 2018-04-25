//
// Created by Semyon Tikhonenko on 4/22/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxFile.h"
#include <fstream>
#include "Strings.h"

using namespace CppUtils;

void MvxFile::writeToFile(const VxFile& vxFile, const char* instrumentalFilePath,
        double beatsPerMinute, const char* outFilePath) {
    std::fstream file = Streams::OpenFile(outFilePath, std::ios::binary | std::ios::out);
    writeToStream(vxFile, instrumentalFilePath, beatsPerMinute, file);
}

void MvxFile::writeToStream(const VxFile& vxFile, const char* instrumentalFilePath,
        double beatsPerMinute, std::ostream& os) {
    os << beatsPerMinute;
    vxFile.writeToStream(os);
    // add space between vxFile Data and instrumental data
    os<<' ';
    
    std::string instrumental = Strings::ReadBinaryFileContent(instrumentalFilePath);
    os.write(instrumental.data(), instrumental.size());
}
