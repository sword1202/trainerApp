//
// Created by Semyon Tykhonenko on 8/31/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa

extension NSView {
    var context: CGContext {
        get {
            return NSGraphicsContext.current!.cgContext
        }
    }
}
