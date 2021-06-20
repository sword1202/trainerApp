//
// Created by Semyon Tikhonenko on 5/16/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import SwiftUI

private let numberOfPreviewSamples = 38

class SongCompletionViewModel : ObservableObject {
    private let projectController = ProjectController.shared
    private let flow: SongCompletionFlowBridge
    @Published var shouldNavigateToRecordings: Bool = false
    @Binding var isActive: Bool {
        didSet {
            if (!isActive) {
                flow.tryAgain()
            }
        }
    }

    init(flow: SongCompletionFlowBridge, isActive: Binding<Bool>) {
        self.flow = flow
        _isActive = isActive
    }

    func getRecordingPreviewImage() -> UIImage {
        let samples = projectController.getRecordingPreviewSamples(numberOfSamples: numberOfPreviewSamples).map {
            $0.floatValue
        }
        return UiUtils.drawRecordingPreviewTrack(samples: samples)
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
}
