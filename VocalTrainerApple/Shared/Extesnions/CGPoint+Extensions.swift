//
// Created by Semyon Tykhonenko on 2019-06-19.
//

extension CGPoint {
    func movedBy(y: CGFloat) -> CGPoint {
        return CGPoint(x: self.x, y: self.y + y)
    }

    static func + (left: CGPoint, right: CGSize) -> CGPoint {
        return CGPoint(x: left.x + right.width, y: left.y + right.height)
    }
}