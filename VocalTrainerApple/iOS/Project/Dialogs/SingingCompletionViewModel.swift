//
// Created by Semyon Tikhonenko on 5/16/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import SwiftUI

private let numberOfPreviewSamples = 38

class SingingCompletionViewModel: ObservableObject {
    private var projectController: ProjectController!
    private var flow: SingingCompletionFlowBridge!
    @Published var shouldNavigateToRecordings: Bool = false
    @Published var recordingPreviewImage: UIImage?
    private var tryAgainOnDeinit = true

    func configure(projectController: ProjectController, flow: SingingCompletionFlowBridge) {
        guard self.projectController == nil else {
            return
        }

        self.flow = flow
        self.projectController = projectController
        recordingPreviewImage = getRecordingPreviewImage()
    }

    private func getRecordingPreviewImage() -> UIImage {
        let samples = projectController.getRecordingPreviewSamples(numberOfSamples: numberOfPreviewSamples).map {
            $0.floatValue
        }
        return PreviewSamplesDraw.drawRecordingPreviewTrack(samples: samples, sampleWidth: 3)
    }

    func didTapTryAgain() {
        flow.tryAgain()
    }

    func didTapSave() {
        flow.save()
        shouldNavigateToRecordings = true
    }

    func didTapListen() {
        flow.listen()
    }

    deinit {
        if tryAgainOnDeinit {
            flow.tryAgain()
        }
    }
}
