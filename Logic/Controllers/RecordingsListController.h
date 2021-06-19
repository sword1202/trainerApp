//
// Created by Semyon Tikhonenko on 6/10/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_RECORDINGSLISTCONTROLLER_H
#define VOCALTRAINER_RECORDINGSLISTCONTROLLER_H

#include <stdint.h>
#include "MvxFile.h"

struct Recording {
    uint64_t date;
    std::string filePath;
    MvxFileHeader header;
};

class RecordingsListController {
    std::vector<Recording> recordings;
public:
    RecordingsListController(const char* recordingsPath);
    int getRecordingsCount() const;
    Recording getRecordingAt(int index) const;
};


#endif //VOCALTRAINER_RECORDINGSLISTCONTROLLER_H
