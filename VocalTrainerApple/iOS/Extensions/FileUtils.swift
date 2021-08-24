//
// Created by Semyon Tikhonenko on 8/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

struct FileUtils {
    static func getFileModificationDate(filePath: String?) -> Date? {
        guard let path = filePath  else {
            return nil
        }

        guard let attrs = try? FileManager.default.attributesOfItem(atPath: path) else {
            return nil
        }

        return attrs[FileAttributeKey.modificationDate] as? Date
    }
}
