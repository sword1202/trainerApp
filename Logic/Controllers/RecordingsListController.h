//
// Created by Semyon Tikhonenko on 6/10/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_RECORDINGSLISTCONTROLLER_H
#define VOCALTRAINER_RECORDINGSLISTCONTROLLER_H

#include <stdint.h>
#include "MvxFile.h"
#include "DestructorQueue.h"

struct Recording {
    uint64_t date;
    std::string filePath;
    MvxFileHeader header;
};

class RecordingsListControllerDelegate {
public:
    virtual void updateRecordingsList() = 0;
    virtual ~RecordingsListControllerDelegate() = default;
};

class RecordingsListController : public CppUtils::DestructorQueue {
    std::vector<Recording> recordings;
    std::string recordingsPath;
    RecordingsListControllerDelegate* delegate = nullptr;
    void updateList();
public:
    RecordingsListController(const char *recordingsPath, RecordingsListControllerDelegate *delegate);
    int getRecordingsCount() const;
    Recording getRecordingAt(int index) const;
    std::vector<float> getSamplesForRecordingAt(int index, int samplesCount) const;
    void deleteRecording(int index);
};


#endif //VOCALTRAINER_RECORDINGSLISTCONTROLLER_H
