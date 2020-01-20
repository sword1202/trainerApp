//
// Created by Semyon Tykhonenko on 2019-07-28.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Foundation

import Cocoa

class ColorView: NSView {
    @IBInspectable var backgroundColor:NSColor = NSColor.white

    required init?(coder decoder: NSCoder) {
        super.init(coder: decoder)
        wantsLayer = true
    }

    override init(frame frameRect: NSRect) {
        super.init(frame: frameRect)
        wantsLayer = true
    }

    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)
        backgroundColor.setFill()
        dirtyRect.fill()
    }
}