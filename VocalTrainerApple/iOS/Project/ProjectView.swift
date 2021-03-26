//
//  ContentView.swift
//  VocalTraineriOS
//
//  Created by Semyon Tikhonenko on 12/18/20.
//  Copyright © 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

private let topButtonFrameHeight: CGFloat = 46

private struct TopPanelToggleButton: View {
    let image: String
    @Binding var isSelected: Bool

    var body: some View {
        Button(action: {
            withAnimation {
                isSelected.toggle()
            }
        }) {
            Image(image)
        }.frame(width: topButtonFrameHeight + 4, height: topButtonFrameHeight)
                .background(isSelected ? Colors.tone3 : Color.white.opacity(0.0))
                .cornerRadius(3)
    }
}

private struct TwoLinesButton: View {
    @Binding var isSelected: Bool
    @Binding var topText: String
    let bottomText: String
    let width: CGFloat

    var body: some View {
        Button(action: {
            withAnimation {
                isSelected.toggle()
            }
        }) {
            VStack(spacing: 3) {
                Text(topText)
                        .font(Font.system(size: 16, weight: .semibold))
                        .foregroundColor(Color.white)
                Text(bottomText.uppercased())
                        .font(Font.system(size: 9, weight: .regular))
                        .foregroundColor(Color.white)
            }
        }.frame(width: width, height: topButtonFrameHeight)
                .background(isSelected ? Colors.tone3 : Color.white.opacity(0.0))
                .cornerRadius(3)
    }
}

struct ProjectView: View {
    @Environment(\.scenePhase) private var scenePhase
    @StateObject private var viewModel = ProjectViewModel()
    @StateObject private var tonalityViewModel = TonalityViewModel()
    @StateObject private var tempoViewModel = TempoViewModel()
    @State private var levelsVisible = false
    @State private var tonalityDialogVisible = false
    @State private var tempoDialogVisible = false

    var body: some View {
        ZStack {
            // Fill safe area with colors
            VStack {
                Spacer().frame(maxWidth: .infinity, maxHeight: 50).background(Colors.tone2)
                Spacer().frame(maxWidth: .infinity).background(Color.white)
                Spacer().frame(maxWidth: .infinity, maxHeight: 50).background(Colors.tone1)
            }.edgesIgnoringSafeArea(.bottom).edgesIgnoringSafeArea(.top)
            // Main Content
            VStack(spacing: 0) {
                VStack(spacing: 0) {
                    HStack(spacing: 6) {
                        Spacer().frame(maxWidth: .infinity, maxHeight: .infinity)
                        TwoLinesButton(
                                isSelected: $tonalityDialogVisible,
                                topText: $tonalityViewModel.tonality,
                                bottomText: Strings.key.localized,
                                width: topButtonFrameHeight + 12)
                        TwoLinesButton(
                                isSelected: $tempoDialogVisible,
                                topText: $tempoViewModel.bpm,
                                bottomText: Strings.tempo.localized,
                                width: topButtonFrameHeight + 4)
                        TopPanelToggleButton(image: "LevelsButton", isSelected: $levelsVisible)
                        TopPanelToggleButton(image: "LyricsToggleButton", isSelected: $viewModel.isLyricsVisible)
                        TopPanelToggleButton(image: "MetronomeButton", isSelected: $viewModel.isMetronomeEnabled)
                    }.frame(maxWidth: .infinity, maxHeight: topButtonFrameHeight).padding(.trailing, 8)
                    Text(viewModel.title)
                            .font(Font.system(size: 18, weight: .bold))
                            .foregroundColor(Color.white)
                            .frame(maxWidth: .infinity, alignment: .leading)
                            .padding(.top, 8)
                            .padding(.bottom, 8)
                            .padding(.leading, 16)
                }.background(Colors.tone2).frame(maxWidth: .infinity, alignment: .topLeading)
                ZStack(alignment: .bottom) {
                    WorkspaceView().onChange(of: scenePhase) { phase in
                        switch phase {
                        case .active:
                            viewModel.didBecomeActive()
                        case .inactive:
                            viewModel.willBecomeInactive()
                        case .background:
                            print("App goes background")
                        default:
                            print("Unknown state")
                        }
                    }

                    ZStack {
                        if (viewModel.isLyricsVisible) {
                            LyricsView(lines: $viewModel.lyricsLines, lyricsSelection: $viewModel.lyricsSelection)
                                    .frame(maxWidth: .infinity, maxHeight: 82)
                                    .background(Colors.tone2)
                        }
                    }
                }
                VStack(alignment: .center) {
                    PlaybackSlider(
                            progress: $viewModel.progress,
                            sections: $viewModel.playbackSections,
                            currentTime: $viewModel.playbackCurrentTime,
                            endTime: $viewModel.playbackEndTime)
                            .padding(.leading, 16)
                            .padding(.trailing, 16)
                            .padding(.top, 14)
                    Spacer().frame(maxWidth: .infinity)
                    HStack {
                        Image("RetryButton")
                        Button(action: {
                            viewModel.didTapPlayButton()
                        }) {
                            Image("PlayButton")
                        }.padding(.leading, 50).padding(.trailing, 50)
                        Image("SelectBoundsButton")
                    }.padding(.bottom, 0)
                }.background(Colors.tone1).frame(maxWidth: .infinity,
                        maxHeight: viewModel.playbackSections.count > 1 ? 124 : 112,
                        alignment: .bottomLeading)
            }.onChange(of: levelsVisible) {
                if ($0) {
                    tempoDialogVisible = false
                    tonalityDialogVisible = false
                }
            }.onChange(of: tempoDialogVisible) {
                if ($0) {
                    levelsVisible = false
                    tonalityDialogVisible = false
                }
            }.onChange(of: tonalityDialogVisible) {
                if ($0) {
                    tempoDialogVisible = false
                    levelsVisible = false
                }
            }
            // Dialogs
            // Put all the dialogs into zstacks to make the animation work
            ZStack {
                if levelsVisible {
                    LevelsDialog(isShown: $levelsVisible.animation()).transition(.move(edge: .bottom))
                }
            }
            ZStack {
                if tonalityDialogVisible {
                    TonalityDialog(viewModel: tonalityViewModel, isShown: $tonalityDialogVisible).transition(.move(edge: .bottom))
                }
            }
            ZStack {
                if tempoDialogVisible {
                    TempoDialog(viewModel: tempoViewModel, isShown: $tempoDialogVisible).transition(.move(edge: .bottom))
                }
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ProjectView()
    }
}
