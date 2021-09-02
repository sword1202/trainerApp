//
// Created by Semyon Tikhonenko on 9/2/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

extension PlaybackSource {
    static func training() -> PlaybackSource {
        let mvxFilePath = Bundle.main.path(forResource: "drm", ofType: "mvx")
        return PlaybackSource(filePath: mvxFilePath)
    }
}