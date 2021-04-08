//
// Created by Semyon Tikhonenko on 3/26/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

private struct LyricsSectionView : View {
    @ObservedObject private var viewModel: BoundsSelectionViewModel
    private let sectionIndex: Int

    init(viewModel: BoundsSelectionViewModel, sectionIndex: Int) {
        self.viewModel = viewModel
        self.sectionIndex = sectionIndex
    }

    var body: some View {
        VStack(alignment: .leading, spacing: 0) {
            Text(viewModel.getSection(at: sectionIndex))
                    .foregroundColor(Colors.tone7)
                    .font(Font.system(size: 24, weight: .bold))
                    .padding(.leading, 14)
                    .padding(.bottom, 12)
                    .padding(.top, sectionIndex == 0 ? 18 : 12)
            ForEach(0..<viewModel.getLinesCount(inSection: sectionIndex), id: \.self) {
                let lineIndex = $0 + viewModel.getSectionFirstLineIndex(sectionIndex: sectionIndex)
                let line = viewModel.getLine(index: lineIndex)

                HStack(spacing: 0) {
                    CheckBox(isSelected: $viewModel.lineSelectionStates[lineIndex])
                    Text(line).font(Font.system(size: 18, weight: .semibold))
                            .foregroundColor(Colors.tone5)
                            .padding(.leading, 18)
                }.frame(maxWidth: .infinity, alignment: .leading).padding(16).background(viewModel.lineSelectionStates[lineIndex] ? Colors.tone1 : Color.white).onTapGesture {
                    viewModel.didTapOnLine(lineIndex: lineIndex)
                }
            }
        }.frame(maxWidth: .infinity, alignment: .leading)
    }
}

struct BoundsSelectionDialog : View {
    private let safeAreaInsets: EdgeInsets
    @Binding var isShown: Bool

    @StateObject private var viewModel = BoundsSelectionViewModel()

    init(safeAreaInsets: EdgeInsets, isShown: Binding<Bool>) {
        _isShown = isShown
        self.safeAreaInsets = safeAreaInsets
    }

    var body: some View {
        VStack(alignment: .leading, spacing: 0) {
            Spacer().frame(height: 96)
            BottomDialogHeader(
                    title: Strings.lyrics.localized,
                    titleIcon: "LyricsToggleButton",
                    isShown: $isShown).frame(maxWidth: .infinity)

            ZStack {
                ScrollView {
                    VStack(alignment: .leading, spacing: 0) {
                        ForEach(0..<viewModel.getSectionsCount(), id: \.self) {
                            LyricsSectionView(viewModel: viewModel, sectionIndex: $0)
                        }
                    }
                }
                VStack {
                    Spacer()
                    ZStack {
                        if viewModel.isLoopItAvailable {
                            BigButton(text: Strings.loopIt.localized, action: {
                                viewModel.didTapLoopIt()
                                isShown = false
                            }).frame(maxWidth: .infinity)
                                    .padding(.leading, 16).padding(.trailing, 16)
                                    .padding(.bottom, max(safeAreaInsets.bottom + 2, 16))
                        }
                    }.background(LinearGradient(gradient: Gradient(colors: [
                        Color.white.opacity(0.0), Color.white
                    ]), startPoint: .top, endPoint: .bottom)).frame(maxWidth: .infinity)
                }
            }.background(Color.white)
        }.ignoresSafeArea(edges: .bottom)
    }
}
