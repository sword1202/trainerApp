//
// Created by Semyon Tikhonenko on 3/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

private struct LevelSlider: View {
    @Binding var value: Float

    var body: some View {
        Slider(value: $value, in: 0.0...1.0, step: 0.01,
                minimumValueLabel: Image("MinVolume").padding(.trailing, 4).padding(.top, 1),
                maximumValueLabel: Image("MaxVolume").padding(.leading, 2).padding(.top, 1)) {
            Text("Volume")
        }.accentColor(Colors.tone2)
    }
}

private struct LevelSection : View {
    private var label: String
    @Binding var value: Float

    init(label: String, value: Binding<Float>) {
        self.label = label
        _value = value
    }

    var body: some View {
        VStack {
            VStack(alignment: .leading) {
                Text(label).font(Font.system(size: 20, weight: .semibold)).foregroundColor(Colors.tone5)
                LevelSlider(value: $value).padding(.top, 22)
            }.padding(.leading, 16).padding(.trailing, 16).padding(.top, 28).padding(.bottom, 28)
            Divider()
        }
    }
}

struct LevelsDialog : View {
    @Binding var instrumentalLevel: Float
    @Binding var vocalLineLevel: Float
    @Binding var voiceLevel: Float
    @Binding var isShown: Bool

    var body: some View {
        BottomDialog(title: "Levels", titleIcon:"LevelsButton", isShown: $isShown) {
            VStack{
                LevelSection(label: Strings.instrumentalLevelLabel.localized, value: $instrumentalLevel)
                LevelSection(label: Strings.vocalLineLevelLabel.localized, value: $vocalLineLevel)
                if (voiceLevel >= 0) {
                    LevelSection(label: Strings.voiceLevelLabel.localized, value: $voiceLevel)
                }
            }.background(Color.white)
        }
    }
}
