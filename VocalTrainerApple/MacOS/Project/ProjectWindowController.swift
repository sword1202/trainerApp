//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import Cocoa

class ProjectWindowController : NSWindowController {
    override func windowDidLoad() {
        super.windowDidLoad()
        window?.setFrame(NSScreen.main!.visibleFrame, display: true)
    }
}
