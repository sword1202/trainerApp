//
// Created by Semyon Tikhonenko on 6/20/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

private let sampleWidth: CGFloat = 4.7

class RecordingsViewModel : ObservableObject {
    private let recordingsController = RecordingsListController()
    @Published private(set) var recordingsCount: Int

    init() {
        recordingsCount = recordingsController.recordingsCount()
    }

    func getPreviewSamplesImage(at index: Int, width: CGFloat) -> UIImage {
        let samplesCount = PreviewSamplesDraw.calculateSamplesCount(width: width, sampleWidth: sampleWidth)
        let samples = recordingsController.getPreviewSamples(recordingIndex: index, samplesCount: samplesCount).map {
            $0.floatValue
        }
        return PreviewSamplesDraw.drawRecordingPreviewTrack(
                samples: samples,
                sampleWidth: sampleWidth,
                color: Colors.recordingBackgroundTrack.cgColor)
    }

    func getRecording(at index: Int) -> RecordingInfo {
        recordingsController.getRecording(at: index)
    }

    func deleteRecording(at index: Int) {
        recordingsController.deleteRecording(at: index)
        recordingsCount = recordingsController.recordingsCount()
    }
}
