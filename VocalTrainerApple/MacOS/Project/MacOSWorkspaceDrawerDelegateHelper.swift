//
// Created by Semyon Tykhonenko on 8/24/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa
import SwiftHEXColors

class MacOSWorkspaceDrawerDelegateHelper : NSObject {
    @objc static func drawTrackButton(text: String, size: CGSize, scaleFactor: CGFloat) -> NSImage {
        return NSImage.draw(size: size, scaleFactor: scaleFactor, drawer: { ctx in
            let color = NSColor(hex: 0x514E64)!
            ctx.setStrokeColor(color.cgColor)
            let rect = NSRect(origin: .zero, size: size).insetBy(dx: 1, dy: 1)
            NSColor.white.setFill()
            ctx.fill(roundedRect: rect, cornerRadius: 2.5)
            NSBezierPath(roundedRect: rect, xRadius: 2.5, yRadius: 2.5).stroke()
            ctx.draw(text: text, centredIn: rect, font: NSFont.systemFont(ofSize: 11), textColor: color)
        })
    }
}
