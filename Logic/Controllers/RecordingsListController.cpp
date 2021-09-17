//
// Created by Semyon Tikhonenko on 6/10/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#include "RecordingsListController.h"
#include "FileUtils.h"
#include "Algorithms.h"
#include "Executors.h"
#include "AudioUtils.h"
#include "InterControllerCommunicationEvents.h"

using namespace CppUtils;

void RecordingsListController::updateList() {
    std::error_code errorCode;
    std::filesystem::directory_iterator iter(recordingsPath, errorCode);
    if (errorCode) {
        return;
    }

    recordings.clear();
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

RecordingsListController::RecordingsListController(const char *recordingsPath, RecordingsListControllerDelegate *delegate)
        : delegate(delegate) {
    this->recordingsPath = recordingsPath;
    updateList();

    InterControllerCommunicationEvents::instance().onNewRecordingAdded.addListener([=] {
        updateList();
        this->delegate->updateRecordingsList();
    }, this);
}

int RecordingsListController::getRecordingsCount() const {
    return static_cast<int>(recordings.size());
}

Recording RecordingsListController::getRecordingAt(int index) const {
    return recordings.at(static_cast<size_t>(index));
}

std::vector<float> RecordingsListController::getSamplesForRecordingAt(int index, int samplesCount) const {
    const std::vector<short> &samples = getRecordingAt(index).header.recordingPreviewSamples;
    return AudioUtils::ResizePreviewSamplesIntoFloatSamples(
            samples.data(),
            static_cast<int>(samples.size()),
            samplesCount);
}

void RecordingsListController::deleteRecording(int index) {
    assert(index >= 0 && index < recordings.size());
    const std::string& filePath = recordings[index].filePath;
    Executors::ExecuteOnBackgroundThread([=] {
        std::filesystem::remove(filePath);
    });
    recordings.erase(recordings.cbegin() + index);
}
