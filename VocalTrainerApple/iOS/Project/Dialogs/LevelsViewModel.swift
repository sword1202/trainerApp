//
// Created by Semyon Tikhonenko on 3/25/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import AVFoundation

class LevelsViewModel : ObservableObject {
    private let projectController: ProjectController

    @Published var instrumentalLevel: Float = 1.0 {
        didSet {
            projectController.setInstrumentalVolume(instrumentalLevel)
        }
    }

    @Published var vocalLineLevel: Float = 1.0 {
        didSet {
            projectController.setVocalPianoVolume(vocalLineLevel)
        }
    }

    @Published var voiceLevel: Float  {
        didSet {
            if !projectController.isRecording {
                try! AVAudioSession.sharedInstance().setInputGain(voiceLevel)
            }
            projectController.setVocalVolume(voiceLevel)
        }
    }

    let hasVoiceSection: Bool

    init(projectController: ProjectController) {
        self.projectController = projectController
        if projectController.isRecording {
            voiceLevel = 1.0
            hasVoiceSection = true
        } else {
            let audioSession = AVAudioSession.sharedInstance()
            hasVoiceSection = audioSession.isInputGainSettable
            voiceLevel = audioSession.inputGain
        }

        if !SwiftUIUtils.isPreview() {
            projectController.add(delegate: self)
        }
    }

    deinit {
        if !SwiftUIUtils.isPreview() {
            projectController.remove(delegate: self)
        }
    }
}

extension LevelsViewModel : ProjectControllerBridgeDelegate {
    func projectControllerUpdate(vocalPianoVolume: Float) {
        self.vocalLineLevel = vocalPianoVolume
    }

    func projectControllerUpdate(instrumentalVolume: Float) {
        self.instrumentalLevel = instrumentalVolume
    }
}
