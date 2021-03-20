//
// Created by Semyon Tikhonenko on 3/20/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct ClickGesture: Gesture {
    let count: Int
    let coordinateSpace: CoordinateSpace

    typealias Value = SimultaneousGesture<TapGesture, DragGesture>.Value

    init(count: Int = 1, coordinateSpace: CoordinateSpace = .local) {
        precondition(count > 0, "Count must be greater than or equal to 1.")
        self.count = count
        self.coordinateSpace = coordinateSpace
    }

    var body: SimultaneousGesture<TapGesture, DragGesture> {
        SimultaneousGesture(
                TapGesture(count: count),
                DragGesture(minimumDistance: 0, coordinateSpace: coordinateSpace)
        )
    }

    func onEnded(perform action: @escaping (CGPoint) -> Void) -> _EndedGesture<ClickGesture> {
        ClickGesture(count: count, coordinateSpace: coordinateSpace)
                .onEnded { (value: Value) -> Void in
                    guard value.first != nil else { return }
                    guard let location = value.second?.startLocation else { return }
                    guard let endLocation = value.second?.location else { return }
                    guard ((location.x-1)...(location.x+1)).contains(endLocation.x),
                          ((location.y-1)...(location.y+1)).contains(endLocation.y) else {
                        return
                    }

                    action(location)
                }
    }
}

extension View {
    func onClickGesture(
            count: Int,
            coordinateSpace: CoordinateSpace = .local,
            perform action: @escaping (CGPoint) -> Void
    ) -> some View {
        gesture(ClickGesture(count: count, coordinateSpace: coordinateSpace)
                .onEnded(perform: action)
        )
    }

    func onClickGesture(
            count: Int,
            perform action: @escaping (CGPoint) -> Void
    ) -> some View {
        onClickGesture(count: count, coordinateSpace: .local, perform: action)
    }

    func onClickGesture(
            perform action: @escaping (CGPoint) -> Void
    ) -> some View {
        onClickGesture(count: 1, coordinateSpace: .local, perform: action)
    }
}
