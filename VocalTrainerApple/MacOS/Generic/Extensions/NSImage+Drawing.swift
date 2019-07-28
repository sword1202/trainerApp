//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import Cocoa

extension NSImage {
    static func draw(size: CGSize, drawer:(CGContext)->Void) -> NSImage {
        let context = CGContext(data: nil,
                width: Int(size.width),
                height: Int(size.height),
                bitsPerComponent: 8,
                bytesPerRow: 0,
                space:
                CGColorSpaceCreateDeviceRGB(),
                bitmapInfo: CGImageAlphaInfo.premultipliedLast.rawValue)!

        NSGraphicsContext.saveGraphicsState()
        NSGraphicsContext.current = NSGraphicsContext(cgContext: context, flipped: false)
        drawer(context)
        NSGraphicsContext.restoreGraphicsState()
        return NSImage(cgImage: context.makeImage()!, size: size)
    }

    @objc static func from(text: String, font: NSFont, color: NSColor) -> NSImage {
        let string = NSAttributedString(string: text, attributes: [
            NSAttributedString.Key.font : NSFont(descriptor: font.fontDescriptor,
                    size: font.pointSize * (NSScreen.main?.backingScaleFactor ?? 2.0))!,
            NSAttributedString.Key.foregroundColor : color
        ])

        let size = string.size()
        return draw(size: size) { ctx in
            string.draw(in: NSRect(origin: CGPoint.zero, size: size))
        }
    }
}