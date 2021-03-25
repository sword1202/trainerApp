//
// Created by Semyon Tikhonenko on 3/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

extension SongTonality {
    private func toString(pitchInOctaveIndex: Int) -> String {
        Strings.Pitches.allCases[pitchInOctaveIndex].localized + " " + (isMajor ? Strings.major : Strings.minor).localized
    }

    func toString() -> String {
        toString(pitchInOctaveIndex: self.pitchInOctaveIndex)
    }

    func toString(shiftedBy shift: Int) -> String {
        var index = pitchInOctaveIndex + shift
        let pitchesCount = Strings.Pitches.allCases.count
        if index < 0 {
            index = (-index) % pitchesCount
            index = pitchesCount - index
        } else if index >= pitchesCount {
            index %= pitchesCount
        }

        return toString(pitchInOctaveIndex: index)
    }
}