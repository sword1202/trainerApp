//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

extension Color {
    init(hex: UInt32, alpha: Double = 1.0) {
        let red = Double((hex & 0xff0000) >> 16) / 255.0
        let green = Double((hex & 0xff00) >> 8) / 255.0
        let blue = Double((hex & 0xff) >> 0) / 255.0
        self.init(.sRGB, red: red, green: green, blue: blue, opacity: alpha)
    }

    init(rgbaHex: UInt32) {
        let red = Double((rgbaHex & 0xff000000) >> 24) / 255.0
        let green = Double((rgbaHex & 0xff0000) >> 16) / 255.0
        let blue = Double((rgbaHex & 0xff00) >> 8) / 255.0
        let alpha = Double((rgbaHex & 0xff) >> 0) / 255.0
        self.init(.sRGB, red: red, green: green, blue: blue, opacity: alpha)
    }

    func toRgba() -> (red: UInt8, green: UInt8, blue: UInt8, alpha: UInt8) {
        #if canImport(UIKit)
        typealias NativeColor = UIColor
        #elseif canImport(AppKit)
        typealias NativeColor = NSColor
        #endif

        var r: CGFloat = 0
        var g: CGFloat = 0
        var b: CGFloat = 0
        var a: CGFloat = 0

        guard NativeColor(self).getRed(&r, green: &g, blue: &b, alpha: &a) else {
            // You can handle the failure here as you want
            return (0, 0, 0, 0)
        }

        return ((r * 255.0).roundToUInt8(), (g * 255.0).roundToUInt8(), (b * 255.0).roundToUInt8(), (a  * 255.0).roundToUInt8())
    }

    func toRgbaUInt32() -> UInt32 {
        let rgba = toRgba()
        return (UInt32(rgba.red) << 24) | (UInt32(rgba.green) << 16) | (UInt32(rgba.blue) << 8) | UInt32(rgba.alpha)
    }

    func rotate(angle: CGFloat) -> Color {
        let rotatedRgba = NativeColorUtils.rotateColorPalette(angle, color: toRgbaUInt32())
        return Color(rgbaHex: rotatedRgba)
    }
}
