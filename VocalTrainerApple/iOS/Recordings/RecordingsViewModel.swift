//
// Created by Semyon Tikhonenko on 6/20/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

private let sampleWidth: CGFloat = 4.7

class RecordingsViewModel : ObservableObject {
    private let recordingsController = RecordingsListController()
    @Published private(set) var recordings = [RecordingInfo]()

    init() {
        recordingsController.add(delegate: self)
        recordingsListControllerUpdate()
    }

    func getPreviewSamplesImage(ofRecording recording: RecordingInfo, width: CGFloat) -> UIImage {
        guard let index = recordings.firstIndex(of: recording) else {
            return UIImage()
        }
        let samplesCount = PreviewSamplesDraw.calculateSamplesCount(width: width, sampleWidth: sampleWidth)
        let samples = recordingsController.getPreviewSamples(recordingIndex: index, samplesCount: samplesCount).map {
            $0.floatValue
        }
        return PreviewSamplesDraw.drawRecordingPreviewTrack(
                samples: samples,
                sampleWidth: sampleWidth,
                color: Colors.recordingBackgroundTrack.cgColor)
    }

    func deleteRecording(_ recording: RecordingInfo) {
        guard let index = recordings.firstIndex(of: recording) else {
            return
        }
        recordings.remove(at: index)
        recordingsController.deleteRecording(at: index)
    }

    deinit {
        recordingsController.remove(delegate: self)
    }
}

extension RecordingsViewModel : RecordingsListControllerBridgeDelegate {
    public func recordingsListControllerUpdate() {
        recordings = (0..<recordingsController.recordingsCount()).map {
            recordingsController.getRecording(at: $0)
        }
    }
}
