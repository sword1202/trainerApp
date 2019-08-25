//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Foundation

@objc public protocol ProjectControllerBridgeDelegate {
    func projectControllerUpdate(audioLevel: Double)
    func projectControllerUpdate(seek: Double)
    func projectControllerPlaybackDidStart()
    func projectControllerPlaybackDidStop()
    func projectController(didChangeHasLyrics hasLyrics: Bool)
    func projectControllerUpdate(lyricsText: String)
    func projectController(didChangeLyricsVisibility showLyrics: Bool)
    func projectController(didChangeTracksVisibility showTracks: Bool)
    func projectController(didChangeMetronomeEnabled enabled: Bool)
    func projectControllerUpdate(vocalPianoVolume: Float)
    func projectControllerUpdate(instrumentalVolume: Float)
    func projectControllerUpdate(vocalVolume: Float)
    func projectControllerUpdate(inputSensitivity: Float)
    func projectControllerUpdate(hasSaveIndicator: Bool)
    func projectController(didChangeZoom: Float)
    func projectController(didChangeRewindStatus running: Bool, isBackward: Bool)
}
