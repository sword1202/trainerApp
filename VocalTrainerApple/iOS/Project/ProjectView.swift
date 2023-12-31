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
    let selectedColor: Color
    @Binding var isSelected: Bool

    var body: some View {
        Button(action: {
            withAnimation {
                isSelected.toggle()
            }
        }) {
            Image(image)
        }.frame(width: topButtonFrameHeight + 4, height: topButtonFrameHeight)
                .background(isSelected ? selectedColor : Color.white.opacity(0.0))
                .cornerRadius(3)
    }
}

private struct TwoLinesButton: View {
    @Binding var isSelected: Bool
    @Binding var topText: String
    let bottomText: String
    let width: CGFloat
    let selectedColor: Color

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
                .background(isSelected ? selectedColor : Color.white.opacity(0.0))
                .cornerRadius(3)
    }
}

struct ProjectView: View {
    @Environment(\.scenePhase) private var scenePhase
    @Environment(\.presentationMode) var presentationMode
    @StateObject private var viewModel = ProjectViewModel()
    @StateObject private var tonalityViewModel = TonalityViewModel()
    @StateObject private var tempoViewModel = TempoViewModel()
    @State private var levelsVisible = false
    @State private var tonalityDialogVisible = false
    @State private var tempoDialogVisible = false
    @State private var boundsSelectionDialogVisible = false
    @State private var showSwipeAndZoomSuggestion = AppSettings.shared.showSwipeAndZoomSuggestion
    private let source: PlaybackSource

    init(source: PlaybackSource) {
        self.source = source
        presentationMode.wrappedValue.dismiss()
    }

    var body: some View {
        GeometryReader { geom in
            ZStack {
                // Fill safe area with colors
                VStack {
                    Spacer().frame(maxWidth: .infinity, maxHeight: 50).background(viewModel.tone[1])
                    Spacer().frame(maxWidth: .infinity).background(Color.white)
                    Spacer().frame(maxWidth: .infinity, maxHeight: 50).background(viewModel.tone[0])
                }.edgesIgnoringSafeArea(.bottom).edgesIgnoringSafeArea(.top)
                // Main Content
                VStack(spacing: 0) {
                    VStack(spacing: 0) {
                        HStack(spacing: 6) {
                            Button(action: {
                                presentationMode.wrappedValue.dismiss()
                            }) {
                                Image(systemName: "chevron.left")
                                        .imageScale(.large)
                                        .foregroundColor(Color.white).padding(.leading, 16)
                            }
                            Spacer().frame(maxWidth: .infinity, maxHeight: .infinity)
                            TwoLinesButton(
                                    isSelected: $tonalityDialogVisible,
                                    topText: $tonalityViewModel.tonality,
                                    bottomText: Strings.key.localized,
                                    width: topButtonFrameHeight + 12,
                                    selectedColor: viewModel.tone[2])
                            TwoLinesButton(
                                    isSelected: $tempoDialogVisible,
                                    topText: $tempoViewModel.bpm,
                                    bottomText: Strings.tempo.localized,
                                    width: topButtonFrameHeight + 4,
                                    selectedColor: viewModel.tone[2])
                            TopPanelToggleButton(image: "LevelsButton", selectedColor: viewModel.tone[2], isSelected: $levelsVisible)
                            TopPanelToggleButton(image: "LyricsToggleButton", selectedColor: viewModel.tone[2], isSelected: $viewModel.isLyricsVisible)
                            TopPanelToggleButton(image: "MetronomeButton", selectedColor: viewModel.tone[2], isSelected: $viewModel.isMetronomeEnabled)
                        }.frame(maxWidth: .infinity, maxHeight: topButtonFrameHeight).padding(.trailing, 8)
                        HStack(spacing: 8) {
                            if viewModel.isRecording {
                                Image("RecordingIcon")
                            }
                            Text(viewModel.title)
                                    .font(Font.system(size: 18, weight: .bold))
                                    .foregroundColor(Color.white)
                                    .frame(maxWidth: .infinity, alignment: .leading)
                        }.frame(maxWidth: .infinity, alignment: .leading)
                                .padding(.top, 8)
                                .padding(.bottom, 8)
                                .padding(.leading, 16)

                        if viewModel.isRecording {
                            HStack {
                                Text(viewModel.recordingTimeLabel)
                                        .foregroundColor(Color.white)
                                        .font(Font.system(size: 12))
                                        .frame(width: 148, height: 32, alignment: .center)
                                        .background(RoundedRectangle(cornerRadius: 16).fill(Colors.recordedAgoLabelBackground))
                                Spacer()
                            }.padding(.bottom, 8).padding(.leading, 16)
                        }
                    }.background(viewModel.tone[1]).frame(maxWidth: .infinity, alignment: .topLeading)
                    ZStack {
                        WorkspaceView(projectController: viewModel.projectController).onChange(of: scenePhase) { phase in
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
                            if showSwipeAndZoomSuggestion {
                                VStack(spacing: 24) {
                                    Spacer()
                                    Text(Strings.youCan.localized)
                                            .font(Font.system(size: 32, weight: .semibold))
                                            .foregroundColor(viewModel.tone[4])
                                    HStack(spacing: 46) {
                                        Image("SwipeHelpIcon")
                                        Image("ZoomHelpIcon")
                                    }
                                    Text(Strings.swipeAndZoom.localized)
                                            .font(Font.system(size: 32, weight: .semibold))
                                            .foregroundColor(viewModel.tone[4])
                                    BigButton(text: Strings.gotIt.localized) {
                                        withAnimation {
                                            showSwipeAndZoomSuggestion = false
                                        }
                                        AppSettings.shared.showSwipeAndZoomSuggestion = false
                                    }.padding(.leading, 26).padding(.trailing, 26)
                                    Spacer()
                                }.frame(maxWidth: .infinity)
                                        .background(Color.white.opacity(0.9))
                                        .padding(.leading, WorkspaceView.pianoWidth)
                                        .padding(.top, WorkspaceView.yardstickHeight + 1)
                            }
                        }
                    }
                    if (viewModel.isLyricsVisible) {
                        LyricsView(lines: $viewModel.lyricsLines, lyricsSelection: $viewModel.lyricsSelection)
                                .frame(maxWidth: .infinity, maxHeight: 82)
                                .background(viewModel.tone[1]).transition(.move(edge: .bottom))
                    }
                    VStack(alignment: .center) {
                        PlaybackSlider(
                                progress: $viewModel.progress,
                                sections: $viewModel.playbackSections,
                                currentTime: $viewModel.playbackCurrentTime,
                                endTime: $viewModel.playbackEndTime,
                                tone: viewModel.tone)
                                .padding(.leading, 16)
                                .padding(.trailing, 16)
                                .padding(.top, 14)
                        Spacer().frame(maxWidth: .infinity)
                        HStack {
                            if !viewModel.isRecording {
                                Button(action: {
                                    viewModel.didTapRetry()
                                }) {
                                    ZStack {
                                        Image("RetryButton")
                                        Text(viewModel.retrySeconds.description).font(Font.system(size: 10, weight: .bold))
                                                .foregroundColor(viewModel.tone[2]).offset(y: 2)
                                    }
                                }
                            }
                            Button(action: {
                                viewModel.didTapPlayButton()
                            }) {
                                Image(viewModel.isPlaying ? "PauseButton" : "PlayButton")
                            }.padding(.leading, 50).padding(.trailing, 50)
                            if !viewModel.isRecording {
                                Button(action: {
                                    var showDialog = false
                                    viewModel.didTapBoundsSelection(showBoundsSelectionDialog: &showDialog)
                                    if showDialog {
                                        withAnimation {
                                            boundsSelectionDialogVisible = true
                                        }
                                    }
                                }) {
                                    Image("SelectBoundsButton")
                                }
                            }
                        }.padding(.bottom, 0)
                    }.background(viewModel.tone[0]).frame(maxWidth: .infinity,
                            maxHeight: viewModel.playbackSections.count > 1 ? 124 : 112,
                            alignment: .bottomLeading)
                }.onChange(of: levelsVisible) {
                    if ($0) {
                        tempoDialogVisible = false
                        tonalityDialogVisible = false
                        boundsSelectionDialogVisible = false
                    }
                }.onChange(of: tempoDialogVisible) {
                    if ($0) {
                        levelsVisible = false
                        tonalityDialogVisible = false
                        boundsSelectionDialogVisible = false
                    }
                }.onChange(of: tonalityDialogVisible) {
                    if ($0) {
                        tempoDialogVisible = false
                        levelsVisible = false
                        boundsSelectionDialogVisible = false
                    }
                }.onChange(of: boundsSelectionDialogVisible) {
                    if ($0) {
                        tempoDialogVisible = false
                        tonalityDialogVisible = false
                        levelsVisible = false
                    }
                }.onChange(of: viewModel.showSongCompletionFlow) {
                    if ($0) {
                        tempoDialogVisible = false
                        tonalityDialogVisible = false
                        levelsVisible = false
                        boundsSelectionDialogVisible = false
                    }
                }

                // Dialogs
                // Put all the dialogs into zstacks to make the animation work
                ZStack {
                    if levelsVisible {
                        LevelsDialog(projectController: viewModel.projectController,
                                isShown: $levelsVisible.animation()).transition(.move(edge: .bottom))
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

                ZStack {
                    if boundsSelectionDialogVisible {
                        BoundsSelectionDialog(
                                projectController: viewModel.projectController,
                                screenGeometry: geom,
                                isShown: $boundsSelectionDialogVisible).transition(.move(edge: .bottom))
                    }
                }

                ZStack {
                    if viewModel.showSongCompletionFlow {
                        SingingCompletionDialog(
                                projectController: viewModel.projectController,
                                screenGeometry: geom,
                                isShown: $viewModel.showSongCompletionFlow,
                                flow: viewModel.songCompletionFlow!)
                                .transition(.move(edge: .bottom))
                    }
                }

                NavigationLazyView(ProjectView(source: viewModel.recording!)).navigatePush(whenTrue: $viewModel.showListenScreen)

                NavigationLink(destination: EmptyView()) {
                    EmptyView()
                }
            }
        }.onAppear {
            viewModel.configure(source: source)
            tonalityViewModel.configure(projectController: viewModel.projectController)
            tempoViewModel.configure(projectController: viewModel.projectController)
        }
    }
}
