//
// Created by Semyon Tikhonenko on 4/22/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include "MvxFileWriter.h"
#include <fstream>
#include "Strings.h"

using namespace CppUtils;

void MvxFileWriter::writeToFile(const VxFile& vxFile, const char* instrumentalFilePath, const char* outFilePath) {
    std::fstream file = Streams::OpenFile(outFilePath, std::ios::binary | std::ios::out);
    writeToStream(vxFile, instrumentalFilePath, file);
}

void MvxFileWriter::writeToStream(const VxFile &vxFile, const char *instrumentalFilePath, std::ostream &os) {
    vxFile.writeToStream(os);
    // add space between vxFile Data and instrumental data
    os<<' ';
    
    std::string instrumental = Strings::ReadBinaryFileContent(instrumentalFilePath);
    os.write(instrumental.data(), instrumental.size());
}
