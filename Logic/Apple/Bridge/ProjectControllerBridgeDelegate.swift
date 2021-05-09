//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import CoreGraphics

@objc public protocol ProjectControllerBridgeDelegate {
    @objc optional func projectControllerUpdate(audioLevel: Double)
    @objc optional func projectControllerUpdate(seek: Double)
    @objc optional func projectControllerPlaybackDidStart()
    @objc optional func projectControllerPlaybackDidStop()
    @objc optional func projectControllerUpdateLyricsSelection(
            selectedCharactersCount: Int,
            lastCharacterSelectionPosition: CGFloat,
            lineIndex: Int
    )
    @objc optional func projectControllerUpdate(currentLyricsLines: [String])
    @objc optional func projectControllerUpdate(lyricsVisibility: Bool)
    @objc optional func projectControllerUpdate(tracksVisibility: Bool)
    @objc optional func projectController(didChangeMetronomeEnabled enabled: Bool)
    @objc optional func projectControllerUpdate(vocalPianoVolume: Float)
    @objc optional func projectControllerUpdate(instrumentalVolume: Float)
    @objc optional func projectControllerUpdate(vocalVolume: Float)
    @objc optional func projectControllerUpdate(inputSensitivity: Float)
    @objc optional func projectControllerUpdate(zoom: Float)
    @objc optional func projectController(didChangeRewindStatus running: Bool, isBackward: Bool)
    @objc optional func projectControllerUpdateTonality(pitchShift: Int)
    @objc optional func projectControllerUpdateTempo(factor: Double)
    @objc optional func projectControllerUpdate(endSeek: Double)
    @objc optional func projectControllerPlaybackDidComplete()
}
