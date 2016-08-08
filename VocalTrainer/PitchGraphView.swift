//
// Created by Semyon Tikhonenko on 8/8/16.
// Copyright (c) 2016 Mac. All rights reserved.
//

import Foundation
import AppKit
import SwiftUtils

class PitchGraphView : AppKit.NSView {
    var i:CGFloat = 0;
    var loop:Loop!;

    required override init?(coder: NSCoder) {
        super.init(coder: coder)
        loop = Loop(delay: 0.01, action: {
            [unowned self] in
            self.i++;
            if self.i < 1000 {
                self.display();
            } else {
                self.loop.stop();
            }
        })
        loop.start()
    }

    override func drawRect(dirtyRect: NSRect) {
        let x:CGFloat = 10;
        let y:CGFloat = 10;
        NSColor.redColor().set() // choose color
        let figure = NSBezierPath() // container for line(s)
        figure.moveToPoint(NSMakePoint(x, y)) // start point
        figure.lineToPoint(NSMakePoint(x+10.0 + i, y+10.0 + i)) // destination
        figure.lineWidth = 1  // hair line
        figure.stroke()  // draw line(s) in color
    }
}
