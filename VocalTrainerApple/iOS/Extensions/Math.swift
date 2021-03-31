//
// Created by Semyon Tykhonenko on 2019-07-17.

//

import Foundation

func sqr<T : Numeric>(_ x: T) -> T {
    return x * x
}

func minmax<T : Comparable>(_ a: T, _ b: T) -> (T, T) {
    if a < b {
        return (a, b)
    } else {
        return (b, a)
    }
}