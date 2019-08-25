//
// Created by Semyon Tykhonenko on 8/25/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa

class Button : NSButton {
    var handler: (()->Void)?

    private func postInit() {
        self.wantsLayer = true
        addGestureRecognizer(NSClickGestureRecognizer(target: self, action: #selector(didTap(_:))))
    }

    override init(frame frameRect: NSRect) {
        super.init(frame: frameRect)
        postInit()
    }

    required init?(coder: NSCoder) {
        super.init(coder: coder)
        postInit()
    }

    @objc func didTap(_:NSGestureRecognizer) {
        handler?()
    }
}
