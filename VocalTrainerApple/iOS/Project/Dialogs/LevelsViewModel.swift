//
// Created by Semyon Tikhonenko on 3/25/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import AVFoundation

class LevelsViewModel : ObservableObject {
    private lazy var projectController = ProjectController.shared

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

    @Published var voiceLevel: Float = AVAudioSession.sharedInstance().isInputGainSettable ? 1.0 : -1.0 {
        didSet {
            try! AVAudioSession.sharedInstance().setInputGain(voiceLevel)
        }
    }

    init() {
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
