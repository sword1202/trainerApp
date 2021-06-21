//
// Created by Semyon Tikhonenko on 6/19/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

public class RecordingInfo: NSObject {
    public let date: TimeInterval
    public let filePath: String
    public let songTitle: String
    public let originalArtistName: String

    @objc public init(
        date: TimeInterval,
        filePath: String,
        songTitle: String,
        originalArtistName: String
    ) {
        self.date = date
        self.filePath = filePath
        self.songTitle = songTitle
        self.originalArtistName = originalArtistName
        super.init()
    }
}
