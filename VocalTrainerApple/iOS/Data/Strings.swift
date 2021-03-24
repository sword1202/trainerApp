//
// Created by Semyon Tikhonenko on 3/20/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

enum Strings : String {
    var localized: String {
        NSLocalizedString(self.rawValue, comment: "")
    }

    case chorus
    case verse
    case bridge

    case instrumentalLevelLabel
    case vocalLineLevelLabel
    case voiceLevelLabel

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
}