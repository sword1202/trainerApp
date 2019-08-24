//
// Created by Semyon Tykhonenko on 2019-08-22.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa
import SwiftHEXColors

class HeaderButton : NSButton {
    private func updateShadow() {
        assert(self.layer != nil)
        if self.state == .on {
            self.layer?.removeShadow()
        } else {
            self.layer?.applySketchShadow(
                    color: NSColor(hex: 0x9DA3DC)!,
                    alpha: 0.8,
                    x: 0,
                    y: 2,
                    blur: 3,
                    spread: 0,
                    cornerRadius: 7.5)
        }
    }

    var handler: (()->Void)?

    override var layer: CALayer? {
        didSet {
            // Postpone the shadow update or otherwise the shadow is not updated for some reasons
            DispatchQueue.main.async {
                self.updateShadow()
            }
        }
    }

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
        self.state = self.state == .on ? .off : .on
        handler?()
    }

    override var state: StateValue {
        didSet {
            updateShadow()
        }
    }
}
