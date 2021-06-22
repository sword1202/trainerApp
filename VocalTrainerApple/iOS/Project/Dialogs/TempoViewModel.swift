//
// Created by Semyon Tikhonenko on 3/25/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

private let factorStep: Double = 0.05

class TempoViewModel : ObservableObject {
    private lazy var projectController = ProjectController.shared

    private var factor: Double = 1 {
        didSet {
            projectController.setTempoFactor(factor)
            updateFactorString()
            updateBpm();
        }
    }

    @Published var factorString: String = ""
    @Published var originalBpm: String = ""
    @Published var bpm: String = ""
    @Published var bpmLabel: String = ""

    private func updateBpm() {
        bpm = String(Int(projectController.beatsPerMinute.rounded()))
        bpmLabel = bpm + " " + Strings.bpmSuffix.localized
    }

    private func updateFactorString() {
        var result = factor.description.replacingOccurrences(of: ".", with: ",")

        let factorStringLength = 4
        if (result.count > factorStringLength) {
            result = String(result[...result.index(result.startIndex, offsetBy: factorStringLength - 1)])
        } else {
            while (result.count < factorStringLength) {
                result += "0"
            }
        }

        result += "x"
        factorString = result
    }

    func incrementFactor() {
        factor += factorStep
    }

    func decrementFactor() {
        factor -= factorStep
    }

    func isTempoChangeAllowed() -> Bool {
        !projectController.isRecording
    }

    init() {
        if !SwiftUIUtils.isPreview() {
            updateBpm()
            originalBpm = Strings.originalLabel.localized + " " +
                    String(Int(projectController.originalBeatsPerMinute.rounded()))
            updateFactorString()
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
