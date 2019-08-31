//
// Created by Semyon Tykhonenko on 8/25/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa

class ToggleButton : NSButton {
    override func mouseUp(with event: NSEvent) {
        super.mouseUp(with: event)
        self.state = self.state == .on ? .off : .on
    }
}
