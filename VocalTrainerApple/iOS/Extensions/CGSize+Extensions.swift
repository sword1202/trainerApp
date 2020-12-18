//
// Created by Semyon Tykhonenko on 2019-07-16.
// Copyright (c) 2019 Indiglobal Ltd. All rights reserved.
//

import UIKit
import Foundation

extension CGSize {
    static func * (left: CGSize, right: CGFloat) -> CGSize {
        return CGSize(width: left.width * right, height: left.height * right)
    }

    static func / (left: CGSize, right: CGFloat) -> CGSize {
        return CGSize(width: left.width / right, height: left.height / right)
    }

    static func square(_ size: CGFloat) -> CGSize {
        return CGSize(width: size, height: size)
    }

    static func + (left: CGSize, right: CGFloat) -> CGSize {
        return CGSize(width: left.width + right, height: left.height + right)
    }

    func contains(size: CGSize) -> Bool {
        return size.width <= self.width && size.height <= self.height
    }

    func ceil() -> CGSize {
        CGSize(width: Foundation.ceil(self.width), height: Foundation.ceil(self.height))
    }
}

extension CGSize : AdditiveArithmetic {
    public static func -= (lhs: inout CGSize, rhs: CGSize) {
        lhs.width -= rhs.width
        lhs.height -= rhs.height
    }

    public static func - (lhs: CGSize, rhs: CGSize) -> CGSize {
        return CGSize(width: lhs.width - rhs.width, height: lhs.height - rhs.height)
    }

    public static func += (lhs: inout CGSize, rhs: CGSize) {
        lhs.width += rhs.width
        lhs.height += rhs.height
    }

    public static func + (left: CGSize, right: CGSize) -> CGSize {
        return CGSize(width: left.width + right.width, height: left.height + right.height)
    }
}

extension CGSize : Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(width)
        hasher.combine(height)
    }
}