//
// Created by Semyon Tykhonenko on 8/31/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa

class TouchUpDownButton : NSButton {
    var onMouseUp: (()->Void)?
    var onMouseDown: (()->Void)?

    override func mouseDown(with event: NSEvent) {
        super.mouseDown(with: event)
        onMouseUp?()
    }

    override func mouseUp(with event: NSEvent) {
        super.mouseUp(with: event)
        onMouseDown?()
    }
}