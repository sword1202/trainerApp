//
// Created by Semyon Tykhonenko on 1/21/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "PitchSoundPack.h"
#include "ZipFile.h"
#include "StringUtils.h"
#include "Pitch.h"
#include "WAVFile.h"

using namespace CppUtils;

PitchSoundPack::PitchSoundPack(const char *packFilePath) {
    ZipFile file(packFilePath);
    for (int i = 0; i < file.getEntriesCount(); ++i) {
        const char* name = file.getEntryName(i);
        std::string pitchName = Strings::GetFileNameWithoutExtension(name);
        int frequencyIndex = Pitch(pitchName.data()).getPerfectFrequencyIndex();
        if (frequencyIndex < 0) {
            throw std::runtime_error("Sound pack file has incorrect name, " + pitchName);
        }

        std::string data = file.getEntryData(i);
        if (WAVFile::parseWavHeader(data) != WavConfig()) {
            throw std::runtime_error("For the pitch " + pitchName +
            " file non-default wav config was found, only default is supported for now");
        }
    }
}
