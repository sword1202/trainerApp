//
// Created by Semyon Tykhonenko on 2019-07-21.
// Copyright (c) 2019 ___FULLUSERNAME___. All rights reserved.
//

import Cocoa

class ProjectWindowController : NSWindowController {
    override func windowWillLoad() {
        super.windowWillLoad()
    }

    override func windowDidLoad() {
        super.windowDidLoad()
        self.window?.setFrame(NSScreen.main!.visibleFrame, display: true)
    }
}
