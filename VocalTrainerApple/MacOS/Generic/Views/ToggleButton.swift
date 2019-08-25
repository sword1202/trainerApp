//
// Created by Semyon Tykhonenko on 8/25/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa

class ToggleButton : Button {
    override func didTap(_ recognizer: NSGestureRecognizer) {
        self.state = self.state == .on ? .off : .on
        super.didTap(recognizer)
    }
}
