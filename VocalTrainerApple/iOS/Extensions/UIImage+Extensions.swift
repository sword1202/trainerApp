//
// Created by Semyon Tykhonenko on 2019-06-26.
// Copyright (c) 2019 Indiglobal Ltd. All rights reserved.
//

import UIKit

extension UIImage {
    static func draw(size: CGSize, drawer: (CGContext)->Void) -> UIImage {
        UIGraphicsBeginImageContextWithOptions(size, false, 0.0)
        let context = UIGraphicsGetCurrentContext()
        drawer(context!)
        let img = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext()
        return img!
    }

    @objc static func from(text: String, font: UIFont, textColor: UIColor) -> UIImage {
        let size = font.size(text: text)
        return UIImage.draw(size: size) { ctx in
            ctx.draw(text: text, centredIn: .init(origin: .zero, size: size), font: font, textColor: textColor)
        }
    }

    static func from(color: UIColor, size: CGSize = CGSize(width: 1, height: 1)) -> UIImage {
        draw(size: size, drawer: { context in
            context.setFillColor(color.cgColor)
            context.fill(CGRect(x: 0, y: 0, width: size.width, height: size.height))
        })
    }

    static func from(view: UIView) -> UIImage? {
        UIGraphicsBeginImageContextWithOptions(view.frame.size, view.isOpaque, 0.0)
        view.drawHierarchy(in: view.frame, afterScreenUpdates: true)
        let img = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext()
        return img
    }

    func applyCornerRadius(_ cornerRadius: CGFloat) -> UIImage {
        let imageView = UIImageView()
        imageView.frame = CGRect(origin: .zero, size: self.size)
        imageView.image = self
        imageView.contentMode = .scaleAspectFill
        imageView.layer.masksToBounds = true
        imageView.layer.cornerRadius = cornerRadius

        return imageView.asImage()
    }

    func circular(withSize size: CGFloat) -> UIImage {
        let imageView = UIImageView()
        imageView.frame =  CGRect(x: 0, y: 0, width: size, height: size)
        imageView.image = self
        imageView.contentMode = .scaleAspectFill

        imageView.layer.masksToBounds = true
        imageView.layer.cornerRadius = size / 2

        return imageView.asImage()
    }

    @objc func resized(toPointSize size: CGSize) -> UIImage {
        guard size != self.size else {
            return self
        }

        return UIGraphicsImageRenderer(size: size).image { _ in
            draw(in: CGRect(origin: .zero, size: size))
        }
    }

    func draw(centeredAt: CGPoint, tintColor: UIColor, size: CGSize? = nil) {
        let imageView = UIImageView(image: self)
        imageView.tintColor = tintColor
        if let size = size {
            imageView.frame = CGRect(origin: .zero, size: size)
        } else {
            imageView.sizeToFit()
        }
        let resultImage = imageView.asImage()
        resultImage.draw(centeredAt: centeredAt)
    }

    func draw(centeredAt: CGPoint) {
        draw(at: CGPoint(x: centeredAt.x - self.size.width / 2, y: centeredAt.y - self.size.height / 2))
    }

    func drawInTopRightCorner(inRect rect: CGRect) {
        draw(at: CGPoint(x: rect.width - self.size.width, y: 0))
    }

    enum Anchor {
        case left, right, top, bottom, center
    }

    // Resize UIImage's canvas and fill the empty space with color
    func resizedCanvas(size: CGSize, fillColor: UIColor = UIColor.clear, anchor: Anchor = .center) -> UIImage {
        let newRect = CGRect(origin: CGPoint.zero, size: self.size).insetBy(
                dx: (self.size.width - size.width) / 2, dy: (self.size.height - size.height) / 2)

        return UIImage.draw(size: size, drawer: { ctx in
            ctx.setFillColor(fillColor.cgColor)
            ctx.fill(CGRect(origin: .zero, size: size))

            switch anchor {
            case .center:
                self.draw(centeredAt: newRect.center())
            default:
                fatalError("Not implemented")
            }
        })
    }

    static func gradient(startColor: UIColor, endColor: UIColor, size: CGSize) -> UIImage {
        var startColorRed:CGFloat = 0
        var startColorGreen:CGFloat = 0
        var startColorBlue:CGFloat = 0
        var startAlpha:CGFloat = 0

        if !startColor.getRed(&startColorRed, green: &startColorGreen, blue: &startColorBlue, alpha: &startAlpha) {
            fatalError("Failed")
        }

        var endColorRed:CGFloat = 0
        var endColorGreen:CGFloat = 0
        var endColorBlue:CGFloat = 0
        var endAlpha:CGFloat = 0

        if !endColor.getRed(&endColorRed, green: &endColorGreen, blue: &endColorBlue, alpha: &endAlpha) {
            fatalError("Failed")
        }

        UIGraphicsBeginImageContext(size)

        guard let context = UIGraphicsGetCurrentContext() else {
            UIGraphicsEndImageContext()
            fatalError("Failed")
        }

        UIGraphicsPushContext(context)

        let glossGradient:CGGradient?
        let rgbColorspace:CGColorSpace?
        let num_locations:size_t = 2
        let locations:[CGFloat] = [0.0, 1.0]
        let components:[CGFloat] = [startColorRed, startColorGreen, startColorBlue, startAlpha, endColorRed, endColorGreen, endColorBlue, endAlpha]
        rgbColorspace = CGColorSpaceCreateDeviceRGB()
        glossGradient = CGGradient(colorSpace: rgbColorspace!, colorComponents: components, locations: locations, count: num_locations)
        let topCenter = CGPoint.zero
        let bottomCenter = CGPoint(x: 0, y: size.height)
        context.drawLinearGradient(glossGradient!, start: topCenter, end: bottomCenter, options: CGGradientDrawingOptions.drawsBeforeStartLocation)

        UIGraphicsPopContext()

        guard let gradientImage = UIGraphicsGetImageFromCurrentImageContext() else {
            UIGraphicsEndImageContext()
            fatalError("Failed")
        }

        UIGraphicsEndImageContext()
        return gradientImage
    }

    func resizeProportionally(targetWidth: CGFloat) -> UIImage {
        let ratio = targetWidth / self.size.width
        return self.resized(toPointSize: self.size * ratio)
    }

    func resizeProportionally(targetHeight: CGFloat) -> UIImage {
        let ratio = targetHeight / self.size.height
        return self.resized(toPointSize: self.size * ratio)
    }

    // ratio = width / height
    func crop(targetRatio: CGFloat, position: CGFloat) -> UIImage {
        let currentRatio = self.size.width / self.size.height
        guard currentRatio != targetRatio else {
            return self
        }

        let image = self.cgImage!
        let imageWidth = CGFloat(image.width)
        let imageHeight = CGFloat(image.height)
        
        var rect: CGRect
        if currentRatio < targetRatio {
            let newHeight = imageHeight * currentRatio / targetRatio
            let pointPosition = imageHeight * position
            rect = CGRect(
                    origin: CGPoint(x: 0, y: pointPosition),
                    size: CGSize(width: imageWidth, height: newHeight))
        } else {
            let newWidth = imageWidth * targetRatio / currentRatio
            let pointPosition = imageWidth * position
            rect = CGRect(
                    origin: CGPoint(x: pointPosition, y: 0),
                    size: CGSize(width: newWidth, height: imageHeight))
        }
        
        if rect.maxX > imageWidth {
            rect.origin.x = imageWidth - rect.width
        }
        
        if rect.maxY > imageHeight {
            rect.origin.y = imageHeight - rect.height
        }
        
        let cropped = image.cropping(to: rect)!
        return UIImage(cgImage: cropped)
    }
}
