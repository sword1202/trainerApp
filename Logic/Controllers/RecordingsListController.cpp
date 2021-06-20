//
// Created by Semyon Tikhonenko on 6/10/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#include "RecordingsListController.h"
#include "FileUtils.h"
#include "Algorithms.h"

using namespace CppUtils;

RecordingsListController::RecordingsListController(const char *recordingsPath) {
    std::error_code errorCode;
    std::filesystem::directory_iterator iter(recordingsPath, errorCode);
    if (errorCode) {
        return;
    }

    for (const auto & entry : iter) {
        if (!entry.is_directory()) {
            Recording recording;
            recording.filePath = entry.path().generic_u8string();
            recording.date = FileUtils::GetLastWriteTimeInMicroseconds(entry);
            recording.header.readFromFile(recording.filePath.data());
            recordings.push_back(recording);
        }
    }
    SortByKey(recordings, [] (const Recording& r) {
        return -r.date;
    });
}

int RecordingsListController::getRecordingsCount() const {
    return static_cast<int>(recordings.size());
}

Recording RecordingsListController::getRecordingAt(int index) const {
    return recordings.at(static_cast<size_t>(index));
}
