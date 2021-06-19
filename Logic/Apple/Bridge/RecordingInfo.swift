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
    // array of floats
    public let previewSamples: NSArray

    @objc public init(
        date: TimeInterval,
        filePath: String,
        songTitle: String,
        originalArtistName: String,
        previewSamples: NSArray
    ) {
        self.date = date
        self.filePath = filePath
        self.songTitle = songTitle
        self.originalArtistName = originalArtistName
        self.previewSamples = previewSamples
        super.init()
    }
}
