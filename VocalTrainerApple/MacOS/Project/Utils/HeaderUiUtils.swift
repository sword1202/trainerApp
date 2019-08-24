//
// Created by Semyon Tykhonenko on 8/24/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

enum HeaderUiUtils {
    static func applyShadow(view: NSView, cornerRadius: CGFloat, revert: Bool = false) {
        view.layer?.applySketchShadow(
                color: UI.Color.shadowColor,
                alpha: 0.8,
                x: 0,
                y: revert ? -2 : 2,
                blur: 3,
                spread: 0,
                cornerRadius: cornerRadius)
    }
}
