//
//  ContentView.swift
//  VocalTraineriOS
//
//  Created by Semyon Tikhonenko on 12/18/20.
//  Copyright © 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

private let topButtonFrameSize: CGFloat = 46

struct TopPanelToggleButton: View {
    let image: String
    @Binding var isSelected: Bool

    var body: some View {
        Button(action: {
            isSelected.toggle()
        }) {
            Image(image)
        }.frame(width: topButtonFrameSize, height: topButtonFrameSize)
                .background(isSelected ? Colors.tone3 : Color.white.opacity(0.0))
                .cornerRadius(3)
    }
}

struct ProjectView: View {
    @Environment(\.scenePhase) private var scenePhase
    @ObservedObject private var viewModel = ProjectViewModel()
    @State private var levelsVisible = false

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
                VStack {
                    HStack(spacing: 8) {
                        Spacer().frame(maxWidth: .infinity, maxHeight: .infinity)
                        TopPanelToggleButton(image: "LevelsButton", isSelected: $levelsVisible)
                        TopPanelToggleButton(image: "LyricsToggleButton", isSelected: $viewModel.isLyricsVisible)
                        TopPanelToggleButton(image: "MetronomeButton", isSelected: $viewModel.isMetronomeEnabled)
                    }.frame(maxWidth: .infinity, maxHeight: topButtonFrameSize).padding(.trailing, 8)
                    Spacer().frame(maxWidth: .infinity)
                }.background(Colors.tone2).frame(maxWidth: .infinity, maxHeight: 80, alignment: .topLeading)
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
                if (viewModel.isLyricsVisible) {
                    LyricsView(lines: $viewModel.lyricsLines, lyricsSelection: $viewModel.lyricsSelection)
                            .frame(maxWidth: .infinity, maxHeight: 82)
                            .background(Colors.tone2)
                }
                VStack(alignment: .center) {
                    PlaybackSlider(progress: $viewModel.progress, sections: $viewModel.playbackSections)
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
                }.background(Colors.tone1).frame(maxWidth: .infinity, maxHeight: 112, alignment: .bottomLeading)
            }
            if levelsVisible {
                BottomDialog(title: "Tempo", titleIcon:"TempoIcon") {
                    Rectangle().fill(Color.green).frame(height: 100)
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
