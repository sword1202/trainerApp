//
// Created by Semyon Tykhonenko on 2019-07-29.

//

import Foundation

extension Sequence {
    func max<Key: Comparable>(usingKeyProvider keyProvider: (Element)->Key) -> Element? {
        return self.max(by: {
            return keyProvider($0) < keyProvider($1)
        })
    }

    func min<Key: Comparable>(usingKeyProvider keyProvider: (Element)->Key) -> Element? {
        return self.min(by: {
            return keyProvider($0) < keyProvider($1)
        })
    }

    func min<Key: Comparable>(greaterThen: Key, keyProvider: (Element)->Key) -> Element? {
        // TODO: Should be optimized
        return self.sorted(by: {
            keyProvider($0) < keyProvider($1)
        }).first(where: {
            keyProvider($0) > greaterThen
        })
    }

    func min<Key: Comparable>(lessThen: Key, keyProvider: (Element)->Key) -> Element? {
        guard let element = self.min(usingKeyProvider: keyProvider), keyProvider(element) < lessThen else {
            return nil
        }

        return element
    }

    // Transforms a sequence of a type to a sequence of another type, returns nil of at least one transformation produces nil
    func tryMap<To>(_ closure: (Element)->To?) -> [To]? {
        var result: [To] = []
        for item in self {
            if let to = closure(item) {
                result.append(to)
            } else {
                return nil
            }
        }

        return result
    }

    func sum<Key : AdditiveArithmetic>(keyProvider:(Element)->Key) -> Key {
        var result: Key = .zero
        for i in self {
            result += keyProvider(i)
        }

        return result
    }
}

extension Sequence where Element == Float {
    func sum() -> Double {
        var result: Double = 0
        forEach {
            result += Double($0)
        }

        return result
    }

    func average() -> Float {
        var sum: Double = 0
        var count: Int = 0
        forEach {
            sum += Double($0)
            count+=1;
        }

        return Float(sum / Double(count))
    }
}