//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

private let progressSectionHeight: CGFloat = 12
private let progressSectionWidth: CGFloat = 1
private let progressLineHeight: CGFloat = 8
private let progressDotSize: CGFloat = 14

struct PlaybackSection {
    let name: String
    let position: CGFloat

    init(name: String, position: CGFloat) {
        assert(position <= 0 && position >= 1)
        self.name = name
        self.position = position
    }
}

struct PlaybackSlider : View {
    @Binding var progress: CGFloat

    func onProgressClick(location: CGPoint, width: CGFloat) {
        var x = location.x - progressDotSize / 2
        if (x < 0) {
            x = 0
        }
        let maxX = width - progressDotSize
        if (x > maxX) {
            x = maxX
        }
        self.progress = x / maxX
    }

    var body: some View {
        GeometryReader { geometry in
            ZStack(alignment: .leading) {
                RoundedRectangle(cornerRadius: progressLineHeight / 2)
                        .fill(Colors.tone3)
                        .frame(maxWidth: .infinity, maxHeight: progressLineHeight).onClickGesture { (location) -> () in
                            onProgressClick(location: location, width: geometry.size.width)
                        }
                RoundedRectangle(cornerRadius: progressLineHeight / 2)
                        .fill(Colors.tone2)
                        .frame(
                                maxWidth: (geometry.size.width - progressDotSize) * progress + progressDotSize,
                                maxHeight: progressLineHeight
                        ).onClickGesture { (location) -> () in
                            onProgressClick(location: location, width: geometry.size.width)
                        }
                Circle().fill(Colors.tone2).frame(maxWidth: progressDotSize, maxHeight: progressDotSize, alignment: .leading)
                        .offset(x: (geometry.size.width - progressDotSize) * progress)
            }.frame(width: geometry.size.width, height: progressDotSize)
        }
    }
}