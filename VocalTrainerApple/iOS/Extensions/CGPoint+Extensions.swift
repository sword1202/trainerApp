//
// Created by Semyon Tykhonenko on 2019-06-19.

//

import UIKit

extension CGPoint {
    func movedBy(y: CGFloat) -> CGPoint {
        return CGPoint(x: self.x, y: self.y + y)
    }

    func movedBy(x: CGFloat) -> CGPoint {
        return CGPoint(x: self.x + x, y: self.y)
    }

    static func + (left: CGPoint, right: CGSize) -> CGPoint {
        return CGPoint(x: left.x + right.width, y: left.y + right.height)
    }

    func distance(to point: CGPoint) -> CGFloat {
        return sqrt(sqrDistance(to: point))
    }

    func sqrDistance(to point: CGPoint) -> CGFloat {
        return sqr(point.x - x) + sqr(point.y - y)
    }

    prefix static func - (_ point: CGPoint) -> CGPoint {
        CGPoint(x: -point.x, y: -point.y)
    }

    static func findPointOnLine(distanceFromA: CGFloat, A: CGPoint, B: CGPoint) -> CGPoint {
        let segmentDistance = A.distance(to: B)
        let ratio: CGFloat = distanceFromA / segmentDistance
        let r = 1 - ratio
        return CGPoint(x: r * A.x + ratio * B.x, y: r * A.y + ratio * B.y)
    }

    static func findMidPoint(betweenA A: CGPoint, andB B: CGPoint) -> CGPoint {
        return CGPoint(x: (A.x + B.x) / 2, y: (A.y + B.y) / 2)
    }
    
    static func * (a: CGPoint, value: CGFloat) -> CGPoint {
        return CGPoint(x: a.x * value, y: a.y * value)
    }
}

extension CGPoint : AdditiveArithmetic {
    public static func -= (lhs: inout CGPoint, rhs: CGPoint) {
        lhs.x -= rhs.x
        lhs.y -= rhs.y
    }

    public static func - (lhs: CGPoint, rhs: CGPoint) -> CGPoint {
        return CGPoint(x: lhs.x - rhs.x, y: lhs.y - rhs.y)
    }

    public static func += (lhs: inout CGPoint, rhs: CGPoint) {
        lhs.x += rhs.x
        lhs.y += rhs.y
    }

    public static func + (lhs: CGPoint, rhs: CGPoint) -> CGPoint {
        return CGPoint(x: lhs.x + rhs.x, y: lhs.y + rhs.y)
    }
}
