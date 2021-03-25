//
// Created by Semyon Tikhonenko on 3/25/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

class TempoViewModel : ObservableObject {
    private lazy var projectController = ProjectController.shared

    private var factor: Double = 1 {
        didSet {
            projectController.setTempoFactor(factor)
            updateBpm();
        }
    }

    @Published var originalBpm: Int = 0
    @Published var bpm: Int = 0

    private func updateBpm() {
        bpm = Int(projectController.beatsPerMinute.rounded())
    }

    init() {
        if !SwiftUIUtils.isPreview() {
            updateBpm()
            originalBpm = Int(projectController.originalBeatsPerMinute.rounded())
            projectController.add(delegate: self)
        }
    }

    deinit {
        if !SwiftUIUtils.isPreview() {
            projectController.remove(delegate: self)
        }
    }
}

extension TempoViewModel : ProjectControllerBridgeDelegate {
    func projectControllerUpdateTempo(factor: Double) {
        self.factor = factor
    }
}
