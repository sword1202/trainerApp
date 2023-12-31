//
// Created by Semyon Tikhonenko on 3/20/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

protocol Localizable {
    var localized: String { get }
}

extension Localizable  where Self : RawRepresentable, Self.RawValue == String {
    var localized: String {
        NSLocalizedString(self.rawValue, comment: "")
    }
}

enum Strings : String, Localizable {
    case tempo
    case levels
    case key

    case instrumentalLevelLabel
    case vocalLineLevelLabel
    case voiceLevelLabel

    case chorus
    case verse
    case bridge

    case originalLabel
    case bpmSuffix

    case lyrics

    static func from(sectionType: LyricsSectionType) -> Strings {
        switch (sectionType) {
        case .CHORUS:
            return chorus
        case .VERSE:
            return verse
        case .BRIDGE:
            return bridge
        }
    }

    enum Pitches : String, Localizable, CaseIterable {
        case C, Cs, D, Eb, E, F, Fs, G, Gs, A, Bb,	B
    }

    case major, minor

    case loopIt

    case youCan, swipeAndZoom
    case gotIt

    case tempoChangeWarning

    case lyricsSuggestion

    case listenToTheResult
    case listenToTheResultDescription
    case tryAgain
    case save
    case listen

    case recordings
    case noRecordingsMessage

    case recordedAgoLabel
    case justNow
}