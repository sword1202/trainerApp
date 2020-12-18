//
// Created by Semyon Tykhonenko on 2019-06-25.
// Copyright (c) 2019 Indiglobal Ltd. All rights reserved.
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
}
