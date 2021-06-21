//
// Created by Semyon Tikhonenko on 6/20/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

class RecordingsViewModel : ObservableObject {
    private let recordingsController = RecordingsListController()
    @Published private(set) var recordingsCount: Int

    init() {
        recordingsCount = recordingsController.recordingsCount()
    }

    func getPreviewSamplesImage(at index: Int, width: CGFloat) -> UIImage {
        let samplesCount = PreviewSamplesDraw.calculateSamplesCount(width: width)
        let samples = recordingsController.getPreviewSamples(recordingIndex: index, samplesCount: samplesCount).map {
            $0.floatValue
        }
        return PreviewSamplesDraw.drawRecordingPreviewTrack(
                samples: samples,
                color: Colors.recordingBackgroundTrack.cgColor)
    }

    func getRecording(at index: Int) -> RecordingInfo {
        return recordingsController.getRecording(at: index)
    }
}
