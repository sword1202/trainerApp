//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

private let progressSectionHeight: CGFloat = 11
private let progressSectionWidth: CGFloat = 1
private let progressLineHeight: CGFloat = 8
private let progressDotSize: CGFloat = 14

struct PlaybackSection {
    let name: String
    let position: CGFloat

    init(name: String, position: CGFloat) {
        assert(position >= 0 && position <= 1)
        self.name = name
        self.position = position
    }
}

struct PlaybackSlider : View {
    @Binding var progress: CGFloat
    @Binding var sections: [PlaybackSection]
    @State private var dotDragTempProgress: CGFloat = -1

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
            VStack {
                ZStack {
                    ForEach(0..<sections.count, id: \.self) {
                        let sectionIndex = $0
                        let section = sections[sectionIndex]
                        let sectionX = section.position * (geometry.size.width - progressDotSize) + progressDotSize / 2
                        Text(section.name).font(Font.system(size: 10, weight: .bold)).foregroundColor(Colors.tone5)
                                .position(x: sectionX)
                    }
                }
                ZStack(alignment: .leading) {
                    RoundedRectangle(cornerRadius: progressLineHeight / 2)
                            .fill(Colors.tone3)
                            .frame(maxWidth: .infinity, maxHeight: progressLineHeight, alignment: .bottomLeading).onClickGesture { (location) -> () in
                                onProgressClick(location: location, width: geometry.size.width)
                            }
                    RoundedRectangle(cornerRadius: progressLineHeight / 2)
                            .fill(Colors.tone2)
                            .frame(
                                    maxWidth: (geometry.size.width - progressDotSize) * progress + progressDotSize,
                                    maxHeight: progressLineHeight,
                                    alignment: .bottomLeading
                            ).onClickGesture { (location) -> () in
                                onProgressClick(location: location, width: geometry.size.width)
                            }
                    ForEach(0..<sections.count, id: \.self) {
                        let sectionIndex = $0
                        let section = sections[sectionIndex]
                        let sectionX = section.position * (geometry.size.width - progressDotSize) + progressDotSize / 2
                        Rectangle().fill(Colors.tone4)
                                .frame(width: progressSectionWidth, height: progressSectionHeight, alignment: .bottom)
                                .offset(x: sectionX, y: -(progressSectionHeight - progressLineHeight) / 2)
                    }
                    Circle().fill(Colors.tone2).frame(maxWidth: progressDotSize, maxHeight: progressDotSize, alignment: .bottomLeading)
                            .offset(x: (geometry.size.width - progressDotSize) * progress)
                            .gesture(DragGesture().onChanged { value in
                                if (dotDragTempProgress < 0) {
                                    dotDragTempProgress = progress
                                }

                                let gestureDotOffset = value.location.x - value.startLocation.x
                                let progressOffset = gestureDotOffset / (geometry.size.width - progressDotSize)
                                progress = (dotDragTempProgress + progressOffset).cutToMatchClosedRange(min: 0, max: 1.0)
                            }.onEnded { value in
                                dotDragTempProgress = -1
                            }).onClickGesture { (location) -> () in
                                onProgressClick(location: location, width: geometry.size.width)
                            }
                }.frame(width: geometry.size.width, height: 14, alignment: .bottom)
            }.frame(height: 26)
        }
    }
}