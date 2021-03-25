//
// Created by Semyon Tikhonenko on 3/25/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

extension Double {
    func isWhole() -> Bool {
        Double(Int(self)) == self
    }
}