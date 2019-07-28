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
                bitmapInfo: CGImageAlphaInfo.premultipliedLast.rawValue)

        drawer(context!)

        return NSImage(cgImage: context!.makeImage()!, size: size)
    }

    @objc static func from(text: String, font: NSFont, color: NSColor) -> NSImage {
        let string = NSAttributedString(string: text, attributes: [
            NSAttributedString.Key.font : font,
            NSAttributedString.Key.foregroundColor : color
        ])

        let size = string.size()
        return draw(size: size) { ctx in
            string.draw(in: NSRect(origin: CGPoint.zero, size: size))
        }
    }
}