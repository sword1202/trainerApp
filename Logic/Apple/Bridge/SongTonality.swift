//
// Created by Semyon Tikhonenko on 3/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

public class SongTonality : NSObject {
    public let pitchInOctaveIndex: Int
    public let isMajor: Bool

    @objc public init(pitchInOctaveIndex: Int, isMajor: Bool) {
        self.pitchInOctaveIndex = pitchInOctaveIndex
        self.isMajor = isMajor
        super.init()
    }
}
