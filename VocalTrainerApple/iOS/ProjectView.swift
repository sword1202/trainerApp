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
        VStack {
            VStack {
                Spacer().frame(maxWidth: .infinity)
            }.background(Colors.mainDark).frame(maxWidth: .infinity, maxHeight: 122, alignment: .topLeading)
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
            VStack(alignment: .center) {
                Spacer().frame(maxWidth: .infinity)
                HStack {
                    Image("RetryButton")
                    Button(action: {
                        viewModel.didTapPlayButton()
                    }) {
                        Image("PlayButton")
                    }.padding(.leading, 51).padding(.trailing, 51)
                    Image("SelectBoundsButton")
                }.padding(.bottom, 33)
            }.background(Colors.main).frame(maxWidth: .infinity, maxHeight: 147, alignment: .bottomLeading)
        }.edgesIgnoringSafeArea(.bottom).edgesIgnoringSafeArea(.top)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ProjectView()
    }
}
