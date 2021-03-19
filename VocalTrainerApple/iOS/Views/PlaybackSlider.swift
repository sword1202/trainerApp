//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

private let progressSectionHeight: CGFloat = 12
private let progressSectionWidth: CGFloat = 1
private let progressLineHeight: CGFloat = 8
private let progressDotRadius: CGFloat = 7

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
    var body: some View {
        ZStack {

        }
    }
}