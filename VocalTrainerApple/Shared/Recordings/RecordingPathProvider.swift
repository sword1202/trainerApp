//
// Created by Semyon Tikhonenko on 5/20/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

class RecordingPathProvider {
    static func getRecordingsPath() -> String {
        NSHomeDirectory() + "/Recordings/"
    }
}
