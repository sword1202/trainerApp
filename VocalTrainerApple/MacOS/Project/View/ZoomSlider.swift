//
// Created by Semyon Tykhonenko on 8/31/19.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa

private let generatedCodeScale: CGFloat = 0.75

class ZoomSlider : NSView {
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
        drawSeeker()
        drawSliderPanel()
    }

    private func drawSeeker() {
        context.saveGState()
        context.scaleBy(x: generatedCodeScale, y: generatedCodeScale)
        //context.translateBy(x: <#T##CGFloat##CoreGraphics.CGFloat#>, y: <#T##CGFloat##CoreGraphics.CGFloat#>)
        _drawSeeker()
        context.restoreGState()
    }

    private func drawSliderPanel() {
        context.saveGState()
        context.scaleBy(x: generatedCodeScale, y: generatedCodeScale)
        _drawSliderPanel()
        context.restoreGState()
    }

    private func _drawSeeker() {
        //// Generated code
        let fillColor = NSColor(red: 0.38, green: 0.373, blue: 0.592, alpha: 1)

        let trianglePath = NSBezierPath()
        trianglePath.move(to: NSPoint(x: 6.28, y: 10.45))
        trianglePath.line(to: NSPoint(x: 10.66, y: 1.68))
        trianglePath.curve(to: NSPoint(x: 10.21, y: 0.34), controlPoint1: NSPoint(x: 10.91, y: 1.19), controlPoint2: NSPoint(x: 10.71, y: 0.59))
        trianglePath.curve(to: NSPoint(x: 9.76, y: 0.24), controlPoint1: NSPoint(x: 10.07, y: 0.27), controlPoint2: NSPoint(x: 9.92, y: 0.24))
        trianglePath.line(to: NSPoint(x: 1, y: 0.24))
        trianglePath.curve(to: NSPoint(x: 0, y: 1.24), controlPoint1: NSPoint(x: 0.45, y: 0.24), controlPoint2: NSPoint(x: 0, y: 0.68))
        trianglePath.curve(to: NSPoint(x: 0.11, y: 1.68), controlPoint1: NSPoint(x: 0, y: 1.39), controlPoint2: NSPoint(x: 0.04, y: 1.54))
        trianglePath.line(to: NSPoint(x: 4.49, y: 10.45))
        trianglePath.curve(to: NSPoint(x: 5.83, y: 10.89), controlPoint1: NSPoint(x: 4.73, y: 10.94), controlPoint2: NSPoint(x: 5.34, y: 11.14))
        trianglePath.curve(to: NSPoint(x: 6.28, y: 10.45), controlPoint1: NSPoint(x: 6.02, y: 10.8), controlPoint2: NSPoint(x: 6.18, y: 10.64))
        trianglePath.close()
        trianglePath.windingRule = .evenOdd
        fillColor.setFill()
        trianglePath.fill()
    }

    private func _drawSliderPanel() {
        //// Generated code
        let context = NSGraphicsContext.current!.cgContext

        //// Color Declarations
        let fillColor = NSColor(red: 0.38, green: 0.373, blue: 0.592, alpha: 1)
        let fillColor2 = NSColor(red: 0.529, green: 0.565, blue: 0.816, alpha: 1)

        //// Page-1
        //// New-Instrumental-Editing-Empty-Screen
        //// Sliders
        //// Horizontal-Zoom-slider
        //// Rectangle-7 Drawing
        NSGraphicsContext.saveGraphicsState()
        context.setAlpha(0.4)

        let rectangle7Path = NSBezierPath(rect: NSRect(x: 2, y: 2, width: 162, height: 1))
        fillColor2.setFill()
        rectangle7Path.fill()

        NSGraphicsContext.restoreGraphicsState()

        //// Oval-Copy-27 Drawing
        let ovalCopy27Path = NSBezierPath(ovalIn: NSRect(x: 0, y: 0, width: 5, height: 5))
        fillColor2.setFill()
        ovalCopy27Path.fill()

        //// Oval-Copy-28 Drawing
        let ovalCopy28Path = NSBezierPath(ovalIn: NSRect(x: 15, y: 0, width: 5, height: 5))
        fillColor2.setFill()
        ovalCopy28Path.fill()


        //// Oval-Copy-29 Drawing
        let ovalCopy29Path = NSBezierPath(ovalIn: NSRect(x: 36, y: 0, width: 5, height: 5))
        fillColor.setFill()
        ovalCopy29Path.fill()

        //// Oval-Copy-30 Drawing
        let ovalCopy30Path = NSBezierPath(ovalIn: NSRect(x: 65, y: 0, width: 5, height: 5))
        fillColor2.setFill()
        ovalCopy30Path.fill()

        //// Oval-Copy-31 Drawing
        let ovalCopy31Path = NSBezierPath(ovalIn: NSRect(x: 105, y: 0, width: 5, height: 5))
        fillColor2.setFill()
        ovalCopy31Path.fill()

        //// Oval-Copy-32 Drawing
        let ovalCopy32Path = NSBezierPath(ovalIn: NSRect(x: 160, y: 0, width: 5, height: 5))
        fillColor2.setFill()
        ovalCopy32Path.fill()
    }
}
