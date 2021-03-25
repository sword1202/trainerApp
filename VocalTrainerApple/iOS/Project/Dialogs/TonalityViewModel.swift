//
// Created by Semyon Tikhonenko on 3/25/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

class TonalityViewModel : ObservableObject {
    private lazy var projectController = ProjectController.shared

    @Published var shift: Int = 0 {
        didSet {
            projectController.setPitchShift(shift)
            tonality = projectController.originalTonality.toString(shiftedBy: shift)
        }
    }

    @Published var originalTonality: String = ""
    @Published var tonality: String = ""

    init() {
        if !SwiftUIUtils.isPreview() {
            originalTonality = projectController.originalTonality.toString()
            tonality = originalTonality
            projectController.add(delegate: self)
        }
    }

    deinit {
        if !SwiftUIUtils.isPreview() {
            projectController.remove(delegate: self)
        }
    }
}

extension TonalityViewModel : ProjectControllerBridgeDelegate {
    func projectControllerUpdateTonality(pitchShift: Int) {
        shift = pitchShift
    }
}
