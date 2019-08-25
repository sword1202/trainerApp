//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import Cocoa

extension NSImage {
    static func draw(size: CGSize, scaleFactor: CGFloat? = nil, drawer:(CGContext)->Void) -> NSImage {
        let scale: CGFloat = scaleFactor ?? NSScreen.main?.backingScaleFactor ?? 2
        let context = CGContext(data: nil,
                width: Int(size.width * scale),
                height: Int(size.height * scale),
                bitsPerComponent: 8,
                bytesPerRow: 0,
                space:
                CGColorSpaceCreateDeviceRGB(),
                bitmapInfo: CGImageAlphaInfo.premultipliedLast.rawValue)!

        NSGraphicsContext.saveGraphicsState()
        NSGraphicsContext.current = NSGraphicsContext(cgContext: context, flipped: false)
        context.concatenate(CGAffineTransform(scaleX: scale, y: scale))
        drawer(context)
        NSGraphicsContext.restoreGraphicsState()
        return NSImage(cgImage: context.makeImage()!, size: size)
    }

    @objc static func from(text: String, font: NSFont, color: NSColor, scaleFactor: CGFloat) -> NSImage {
        let string = NSAttributedString(string: text, attributes: [
            NSAttributedString.Key.font : NSFont(descriptor: font.fontDescriptor,
                    size: font.pointSize * scaleFactor)!,
            NSAttributedString.Key.foregroundColor : color
        ])

        let size = string.size()
        return draw(size: size, scaleFactor: 1) { ctx in
            string.draw(in: NSRect(origin: CGPoint.zero, size: size))
        }
    }
}