//
// Created by Semyon Tykhonenko on 2019-06-25.

//

import UIKit

extension CGRect {
    func center() -> CGPoint {
        return CGPoint(x: size.width / 2 + origin.x, y: size.height / 2 + origin.y)
    }

    var bottomRightCorner: CGPoint {
        get {
            return self.origin + self.size
        }
    }

    func with(height: CGFloat) -> CGRect {
        return CGRect(origin: self.origin, size: CGSize(width: self.width, height: height))
    }

    mutating func setCenter(_ center: CGPoint) {
        let diff = center - self.center()
        self.origin += diff
    }
    
    static func * (a: CGRect, value: CGFloat) -> CGRect {
        return CGRect(origin: a.origin * value, size: a.size * value)
    }

    func insetBy(left: CGFloat = 0, right: CGFloat = 0, top: CGFloat = 0, bottom: CGFloat = 0) -> CGRect {
        CGRect(origin: CGPoint(x: self.minX + left, y: self.minY + top),
                size: CGSize(width: self.width - left - right, height: self.height - top - bottom))
    }
}
