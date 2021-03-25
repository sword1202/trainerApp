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
        assert(position >= 0 && position <= 1)
        self.name = name
        self.position = position
    }
}

struct PlaybackSlider : View {
    @Binding var progress: CGFloat
    @Binding var sections: [PlaybackSection]
    @Binding var currentTime: String
    @Binding var endTime: String
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

    func generateSectionNamesViews(width: CGFloat, indexCondition: @escaping (Int) -> Bool) -> some View {
        ZStack(alignment: .leading) {
            ForEach(0..<sections.count, id: \.self) {
                let sectionIndex = $0
                if indexCondition(sectionIndex) {
                    let section = sections[sectionIndex]
                    let sectionX = section.position * (width - progressDotSize) + progressDotSize / 2
                    Text(section.name).font(Font.system(size: 10, weight: .bold)).foregroundColor(Colors.tone5)
                            .position(x: sectionX, y: 6).layoutPriority(1)
                } else {
                    EmptyView()
                }
            }
        }.frame(width: width, height: 12)
    }

    var body: some View {
        GeometryReader { geometry in
            VStack(spacing: 0) {
                generateSectionNamesViews(width: geometry.size.width, indexCondition: { $0 % 2 == 0 })
                ZStack(alignment: .leading) {
                    RoundedRectangle(cornerRadius: progressLineHeight / 2)
                            .fill(Colors.tone3)
                            .frame(maxWidth: .infinity, maxHeight: progressLineHeight, alignment: .center).onClickGesture { (location) -> () in
                                onProgressClick(location: location, width: geometry.size.width)
                            }
                    RoundedRectangle(cornerRadius: progressLineHeight / 2)
                            .fill(Colors.tone2)
                            .frame(
                                    maxWidth: (geometry.size.width - progressDotSize) * progress + progressDotSize,
                                    maxHeight: progressLineHeight,
                                    alignment: .center
                            ).onClickGesture { (location) -> () in
                                onProgressClick(location: location, width: geometry.size.width)
                            }
                    ForEach(0..<sections.count, id: \.self) {
                        let sectionIndex = $0
                        let section = sections[sectionIndex]
                        let sectionX = section.position * (geometry.size.width - progressDotSize) + progressDotSize / 2
                        let offsetSign: CGFloat = sectionIndex % 2 == 0 ? -1 : +1
                        Rectangle().fill(Colors.tone4)
                                .frame(width: progressSectionWidth, height: progressSectionHeight, alignment: .bottom)
                                .offset(x: sectionX, y: offsetSign * (progressSectionHeight - progressLineHeight) / 2)
                    }
                    Circle().fill(Colors.tone2).frame(maxWidth: progressDotSize, maxHeight: progressDotSize, alignment: .center)
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
                }.frame(width: geometry.size.width, height: progressSectionHeight * 2 - progressLineHeight, alignment: .center)
                ZStack {
                    HStack {
                        Text(currentTime).font(Font.system(size: 10)).foregroundColor(Colors.tone5)
                        Spacer()
                        Text(endTime).font(Font.system(size: 10)).foregroundColor(Colors.tone5)
                    }
                    generateSectionNamesViews(width: geometry.size.width, indexCondition: { $0 % 2 == 1 })
                }
            }
        }
    }
}