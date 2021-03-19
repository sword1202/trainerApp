//
// Created by Semyon Tikhonenko on 3/19/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import CoreGraphics

struct LyricsSelection: CustomStringConvertible {
    let characterIndex: Int
    let position: CGFloat
    let lineIndex: Int

    func isSelected() -> Bool {
        characterIndex > 0 || position > 0 || lineIndex > 0
    }

    var description: String {
        "LyricsSelection(characterIndex: \(characterIndex), position: \(position))"
    }
}