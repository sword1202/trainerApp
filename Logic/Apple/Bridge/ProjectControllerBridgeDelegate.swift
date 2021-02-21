//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Foundation

@objc public protocol ProjectControllerBridgeDelegate {
    @objc optional func projectControllerUpdate(audioLevel: Double)
    @objc optional func projectControllerUpdate(seek: Double)
    @objc optional func projectControllerPlaybackDidStart()
    @objc optional func projectControllerPlaybackDidStop()
    @objc optional func projectController(didChangeHasLyrics hasLyrics: Bool)
    @objc optional func projectControllerUpdate(lyricsText: String)
    @objc optional func projectController(didChangeLyricsVisibility showLyrics: Bool)
    @objc optional func projectController(didChangeTracksVisibility showTracks: Bool)
    @objc optional func projectController(didChangeMetronomeEnabled enabled: Bool)
    @objc optional func projectControllerUpdate(vocalPianoVolume: Float)
    @objc optional func projectControllerUpdate(instrumentalVolume: Float)
    @objc optional func projectControllerUpdate(vocalVolume: Float)
    @objc optional func projectControllerUpdate(inputSensitivity: Float)
    @objc optional func projectControllerUpdate(hasSaveIndicator: Bool)
    @objc optional func projectController(didChangeZoom: Float)
    @objc optional func projectController(didChangeRewindStatus running: Bool, isBackward: Bool)
}
