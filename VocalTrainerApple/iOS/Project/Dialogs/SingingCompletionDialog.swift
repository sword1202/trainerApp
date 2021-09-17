//
// Created by Semyon Tikhonenko on 5/9/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct SingingCompletionDialog: View {
    private let screenGeometry: GeometryProxy
    @Binding var isShown: Bool
    @StateObject private var viewModel = SingingCompletionViewModel()
    private let projectController: ProjectController
    private let flow: SingingCompletionFlowBridge

    init(projectController: ProjectController,
         screenGeometry: GeometryProxy,
         isShown: Binding<Bool>,
         flow: SingingCompletionFlowBridge) {
        self.screenGeometry = screenGeometry
        _isShown = isShown
        self.projectController = projectController
        self.flow = flow
    }

    var body: some View {
        BottomDialog(title: Strings.listenToTheResult.localized, titleIcon:"Headphones", isShown: $isShown) {
            VStack(spacing: 0) {
                Image("SongCompletionDialogIcon").padding(.top, 36).padding(.bottom, 22)
                Text(Strings.listenToTheResultDescription.localized)
                Image(uiImage: viewModel.recordingPreviewImage ?? UIImage()).padding(.top, 38)
                HStack(spacing: 8) {
                    BigButton(text: Strings.save.localized) {
                        viewModel.didTapSave()
                    }
                    BigButton(text: Strings.listen.localized, backgroundColor: Colors.alternativeButton) {
                        viewModel.didTapListen()
                    }
                }.padding(.leading, 16).padding(.trailing, 16).padding(.bottom, 16).padding(.top, 50)
                BigButton(text: Strings.tryAgain.localized,
                        backgroundColor: Colors.fadedButton,
                        textColor: Colors.tone5)
                {
                    viewModel.didTapTryAgain()
                }.moveToBottom(screenGeometry: screenGeometry)
            }.frame(maxWidth: .infinity).background(Color.white)
        }.ignoresSafeArea(edges: .bottom).onAppear {
            viewModel.configure(projectController: projectController, flow: flow)
        }.onChange(of: viewModel.shouldNavigateToRecordings) {
            if ($0) {
                NavigationUtil.popToRootView()
            }
        }
    }
}