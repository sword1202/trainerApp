//
// Created by Semyon Tykhonenko on 2019-07-03.
//

import Cocoa

extension CGContext {
    func drawStrokeLinearGradientPath(colors: (NSColor, NSColor), inRect rect: CGRect, pathCreator: ()->Void) {
        saveGState()
        defer { restoreGState() }
        pathCreator()
        replacePathWithStrokedPath()
        clip()

        let cgColors = [colors.0.cgColor, colors.1.cgColor]
        guard let gradient = CGGradient(colorsSpace: nil, colors: cgColors as CFArray, locations: nil)
                else { return }
        drawLinearGradient(gradient, start: rect.origin, end: rect.bottomRightCorner, options: [])
    }

    func drawStrokeLinearGradientArc(colors: (NSColor, NSColor),
                               inRect rect: CGRect,
                               startAngle: CGFloat, endAngle: CGFloat, clockwise: Bool) {
        assert(rect.width == rect.height)
        drawStrokeLinearGradientPath(colors: colors, inRect: rect, pathCreator: {
            self.addArc(center: rect.center(), radius: rect.height / 2,
                    startAngle: startAngle, endAngle: endAngle, clockwise: clockwise)
        })
    }

    func drawStrokeLinearGradientCircle(colors: (NSColor, NSColor), inRect rect: CGRect) {
        assert(rect.width == rect.height)
        drawStrokeLinearGradientPath(colors: colors, inRect: rect, pathCreator: {
            self.addCircle(inRect: rect)
        })
    }

    func addCircle(inRect rect: CGRect) {
        assert(rect.width == rect.height)
        addArc(center: rect.center(), radius: rect.width / 2, startAngle: 0, endAngle: CGFloat.pi * 2, clockwise: true)
    }

    func addCircle(at center: CGPoint, radius: CGFloat) {
        addCircle(inRect: CGRect(x: center.x - radius, y: center.y - radius, width: radius * 2, height: radius * 2))
    }

    func fill(circleAt center: CGPoint, radius: CGFloat) {
        beginPath()
        addCircle(at: center, radius: radius)
        fillPath()
    }

    func fill(roundedRect: CGRect, cornerRadius: CGFloat) {
        NSBezierPath(roundedRect: roundedRect, xRadius: cornerRadius, yRadius: cornerRadius).fill()
    }

    func draw(text: String, centredVerticallyIn rect: CGRect, textFont: NSFont, textColor: NSColor = NSColor.black) {
        let attributedString = NSAttributedString(string: text, attributes: [
            NSAttributedString.Key.font: textFont,
            NSAttributedString.Key.foregroundColor: textColor
        ])

        attributedString.draw(at: CGPoint(x: rect.origin.x,
                y: (rect.height - attributedString.size().height) / 2))
    }

    func draw(text: String, centredIn rect: CGRect, font: NSFont, textColor: NSColor = NSColor.black) {
        draw(text: text, centredAt: rect.center(), font: font, textColor: textColor)
    }

    func draw(text: String, centredAt center: CGPoint, font: NSFont, textColor: NSColor = NSColor.black) {
        let attributedString = NSAttributedString(text: text, font: font, textColor: textColor)
        attributedString.draw(centredAt: center)
    }
}