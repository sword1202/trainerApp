//
// Created by Semyon Tykhonenko on 2019-07-16.
//

extension CGSize {
    static func * (left: CGSize, right: CGFloat) -> CGSize {
        return CGSize(width: left.width * right, height: left.height * right)
    }

    static func square(_ size: CGFloat) -> CGSize {
        return CGSize(width: size, height: size)
    }
}