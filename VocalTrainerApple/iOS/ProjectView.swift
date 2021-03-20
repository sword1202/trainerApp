//
//  ContentView.swift
//  VocalTraineriOS
//
//  Created by Semyon Tikhonenko on 12/18/20.
//  Copyright © 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct ProjectView: View {
    @Environment(\.scenePhase) private var scenePhase
    @ObservedObject private var viewModel = ProjectViewModel()
    
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
                LyricsView(lines: $viewModel.lyricsLines, lyricsSelection: $viewModel.lyricsSelection)
                        .frame(maxWidth: .infinity, maxHeight: 82)
                        .background(Colors.tone2)
                VStack(alignment: .center) {
                    PlaybackSlider(progress: $viewModel.progress)
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
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ProjectView()
    }
}
