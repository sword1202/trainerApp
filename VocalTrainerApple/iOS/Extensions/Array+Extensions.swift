//
// Created by Semyon Tykhonenko on 2019-07-19.

//

import Foundation

extension Array {
    func binarySearch<Key: Comparable>(lessOrEqualTo searchItem: Element, keyProvider: (Element)->Key) -> Int? {
        var lowerIndex = 0;
        var upperIndex = self.count - 1
        let searchKey = keyProvider(searchItem)

        while true {
            let currentIndex = (lowerIndex + upperIndex) / 2
            let currentKey = keyProvider(self[currentIndex])
            if currentKey == searchKey {
                return currentIndex
            } else if lowerIndex > upperIndex {
                if currentKey < searchKey {
                    return currentIndex
                } else {
                    return nil
                }
            } else {
                if currentKey > searchKey {
                    upperIndex = currentIndex - 1
                } else {
                    lowerIndex = currentIndex + 1
                }
            }
        }
    }

    func safeGet(_ index: Int) -> Element? {
        return index < count ? self[index] : nil
    }

    init(factory: (Int)->Element, count: Int) {
        self.init()
        for i in 0..<count {
            append(factory(i))
        }
    }

    func sorted<T : Comparable>(byKey keyProvider: (Element)->T) -> [Element] {
        return sorted(by: {
            keyProvider($0) < keyProvider($1)
        })
    }
    
    func forEach(startIndex: Int, _ body: (Element)->Void) {
        for i in startIndex..<count {
            body(self[i])
        }
    }
    
    mutating func resize(newSize: Int, fillWith element: Element) {
        if self.count < newSize {
            self.append(contentsOf: Array(repeating: element, count: newSize - self.count))
        } else if self.count > newSize {
            self.removeLast(self.count - newSize)
        }
    }

    mutating func removeFirst(where predicate: (Element)->Bool) -> Element? {
        guard let index = self.firstIndex(where: predicate) else {
            return nil
        }

        return self.remove(at: index)
    }

    func count(predicate: (Element)->Bool) -> Int {
        var count = 0
        for item in self {
            if (predicate(item)) {
                count+=1
            }
        }

        return count
    }
}

extension Array where Element : Equatable & Hashable {
    mutating func removeAll(array: [Element]) {
        let set = Set(array)
        self.removeAll(where: { item in set.contains(item) })
    }

    mutating func remove(_ element: Element) {
        self.removeAll(where: { $0 == element })
    }
}

extension Array {
    func filterRemovingAll<Key : Equatable & Hashable>(array: [Element], compareUsingKeyProvider keyProvider: (Element)->Key) -> [Element] {
        let set = Set(array.map { keyProvider($0) })
        return filter {
            !set.contains(keyProvider($0))
        }
    }

    func filterRemovingAll(array: [Element], compareUsingEqualityPredicate predicate: (Element, Element)->Bool) -> [Element] {
        filter { value in
            !array.contains(where: { predicate(value, $0) })
        }
    }
}

extension Array where Element : Equatable {
    mutating func remove(_ element: Element) {
        self.removeAll(where: { $0 == element })
    }

    func contains(_ element: Element) -> Bool {
        self.contains(where: { $0 == element })
    }
}

extension Array where Element: Hashable {
    func removeDuplicates() -> Array {
        var buffer = Array()
        var added = Set<Element>()
        for elem in self {
            if !added.contains(elem) {
                buffer.append(elem)
                added.insert(elem)
            }
        }
        return buffer
    }
}

extension Array {
    func removeDuplicates(usingEqualityPredicate predicate: (Element, Element)->Bool) -> Array {
        var result: [Element] = []
        for item in self {
            if !result.contains(where: { resultItem in
                predicate(resultItem, item)
            }) {
                result.append(item)
            }
        }

        return result
    }
}
