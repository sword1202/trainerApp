//
// Created by Semyon Tikhonenko on 12/19/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import CoreGraphics

extension CGFloat {
    func cutToMatchClosedRange(min: CGFloat, max: CGFloat) -> CGFloat {
        if self < min {
            return min
        }

        if self > max {
            return max
        }

        return self
    }
}