//
// Created by Semyon Tikhonenko on 3/26/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

private struct LyricsSectionView : View {
    private let sectionType: LyricsSectionType
    private let lines: [String]
    @State private var selectedLinesState: [Bool] = []

    init(sectionType: LyricsSectionType, lines: [String]) {
        self.sectionType = sectionType
        self.lines = lines
        self.selectedLinesState = Array(repeating: false, count: lines.count)
    }

    var body: some View {
        VStack {
            Text(Strings.from(sectionType: sectionType).localized)
                    .foregroundColor(Colors.tone7)
                    .font(Font.system(size: 24, weight: .bold))
            ForEach(0..<lines.count, id: \.self) {
                let lineIndex = $0
                let line = lines[lineIndex]
                HStack {
                    CheckBox(isSelected: $selectedLinesState[lineIndex])
                    Text(line)
                }
            }
        }
    }
}

struct BoundsSelectionDialog : View {
    private let safeAreaInsets: EdgeInsets
    @Binding var isShown: Bool

    init(safeAreaInsets: EdgeInsets, isShown: Binding<Bool>) {
        _isShown = isShown
        self.safeAreaInsets = safeAreaInsets
    }

    var body: some View {
        VStack {
            Spacer().frame(height: 96)
            VStack(spacing: 0) {
                BottomDialogHeader(
                        title: Strings.lyrics.localized,
                        titleIcon: "LyricsToggleButton",
                        isShown: $isShown)
                Spacer().frame(maxWidth: .infinity, maxHeight: safeAreaInsets.bottom)
                        .background(Color.white)
            }

            ZStack {
                ScrollView {

                }
                VStack {
                    Spacer()
                    ZStack {
                        Button(action: {

                        }) {
                            ZStack {
                                RoundedRectangle(cornerRadius: 8).fill(Colors.tone6).frame(height: 50)
                                Text(Strings.loopIt.localized).foregroundColor(Color.white).font(Font.system(size: 17, weight: .semibold))
                            }
                        }.frame(maxWidth: .infinity)
                                .padding(.leading, 16).padding(.trailing, 16)
                                .padding(.bottom, max(safeAreaInsets.bottom + 2, 16))
                    }.background(LinearGradient(gradient: Gradient(colors: [
                        Color.white.opacity(0.0), Color.white
                    ]), startPoint: .top, endPoint: .bottom)).frame(maxWidth: .infinity)
                }
            }.background(Color.white)
        }.ignoresSafeArea(edges: .bottom)
    }
}
