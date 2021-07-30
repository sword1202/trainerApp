//
// Created by Semyon Tikhonenko on 12/19/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import CoreGraphics

extension CGFloat {
    func cutToMatchClosedRange(min: CGFloat, max: CGFloat) -> CGFloat {
        assert(min <= max)
        if self < min {
            return min
        }

        if self > max {
            return max
        }

        return self
    }

    func roundToUInt8() -> UInt8 {
        UInt8(rounded().cutToMatchClosedRange(min: CGFloat(UInt8.min), max: CGFloat(UInt8.max)))
    }
}