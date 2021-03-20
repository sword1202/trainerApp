//
// Created by Semyon Tikhonenko on 3/20/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

@objc public enum LyricsSectionType: Int {
    case CHORUS, VERSE, BRIDGE
}

public class LyricsSection : NSObject {
    public let type: LyricsSectionType
    public let number: Int
    public let seek: Double

    @objc public init(type: LyricsSectionType, number: Int, seek: Double) {
        self.type = type
        self.number = number
        self.seek = seek
        super.init()
    }
}
