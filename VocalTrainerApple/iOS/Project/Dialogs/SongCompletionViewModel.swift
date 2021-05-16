//
// Created by Semyon Tikhonenko on 5/16/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

private let numberOfPreviewSamples = 38

class SongCompletionViewModel : ObservableObject {
    private let projectController = ProjectController.shared

    func getRecordingPreviewImage() -> UIImage {
        let samples = projectController.getRecordingPreviewSamples(numberOfSamples: numberOfPreviewSamples).map {
            $0.floatValue
        }
        return UiUtils.drawRecordingPreviewTrack(samples: samples)
    }
}
