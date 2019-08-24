//
// Created by Semyon Tykhonenko on 2019-08-22.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa

extension CALayer {
    func applySketchShadow(
            color: NSColor,
            alpha: Float,
            x: CGFloat,
            y: CGFloat,
            blur: CGFloat,
            spread: CGFloat,
            cornerRadius: CGFloat = 0) {
        self.masksToBounds = false
        shadowColor = color.cgColor
        shadowOpacity = alpha
        shadowOffset = CGSize(width: x, height: y)
        shadowRadius = blur / 2.0
        if spread == 0 {
            shadowPath = nil
        } else {
            let dx = -spread
            let rect = bounds.insetBy(dx: dx, dy: dx)
            shadowPath = CGPath(roundedRect: rect, cornerWidth: cornerRadius,
                    cornerHeight: cornerRadius, transform: nil)
        }
    }

    func removeShadow() {
        self.shadowPath = nil
        self.shadowRadius = 0
        self.shadowColor = NSColor.clear.cgColor
        self.shadowOpacity = 0
        self.shadowOffset = CGSize(width: 0, height: 0)
    }
}
