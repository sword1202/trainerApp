//
// Created by Semyon Tikhonenko on 6/20/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

class RecordingsViewModel : ObservableObject {
    private let recordingsController = RecordingsListController(previewSamplesCount: 36)
    @Published var recordings: [RecordingInfo] = []

    init() {
        recordings = (0..<recordingsController.recordingsCount()).map {
            recordingsController.getRecording(at: $0)
        }
    }
}
