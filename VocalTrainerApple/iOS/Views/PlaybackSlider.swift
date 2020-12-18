//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import UIKit

private class Progress : UIView {
    var progress: CGFloat = 0 {
        didSet {
            self.setNeedsDisplay()
        }
    }

    override func draw(_ rect: CGRect) {
        super.draw(rect)
        let ctx = UIGraphicsGetCurrentContext()!
        ctx.setFillColor(Colors.mainSemiDark.cgColor!)
        ctx.fill(roundedRect: rect, cornerRadius: rect.height / 2)
    }
}

class PlaybackSlider : UIView {
}
