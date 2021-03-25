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
            shiftString = shift <= 0 ? shift.description : "+" + shift.description
        }
    }

    @Published var shiftString: String = "0"

    @Published var originalTonalityLabel: String = ""
    @Published var tonality: String = ""

    init() {
        if !SwiftUIUtils.isPreview() {
            let originalTonality = projectController.originalTonality.toString()
            originalTonalityLabel = Strings.originalLabel.localized + " " +
                    projectController.originalTonality.toString()
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
