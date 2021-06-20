//
// Created by Semyon Tikhonenko on 6/19/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct RecordingsView : View {
    @StateObject private var viewModel = RecordingsViewModel()

    var body: some View {
        GeometryReader { geom in
            // Fill safe area with colors
            VStack {
                Spacer().frame(maxWidth: .infinity, maxHeight: geom.safeAreaInsets.top).background(Colors.tone2)
                Spacer().frame(maxWidth: .infinity).background(Color.white)
            }.edgesIgnoringSafeArea(.bottom).edgesIgnoringSafeArea(.top)

            // Main content
            HStack {
                Text(Strings.recordings.localized)
                        .font(Font.system(size: 34))
                        .foregroundColor(Color.white)
                        .padding(.leading, 16)
                Spacer()
                NavigationLink(destination: ProjectView(filePath: nil).navigationBarHidden(true)) {
                    Image("WhitePlusButton")
                }.padding(.trailing, 18)
            }.frame(height: 72, alignment: .center).background(Colors.tone2)

            if (viewModel.recordings.count > 0) {

            } else {
                VStack(spacing: 16) {
                    Spacer()
                    Image("NoRecordings")
                    Text(Strings.noRecordingsMessage.localized)
                            .frame(width: 180)
                            .multilineTextAlignment(.center)
                            .font(Font.system(size: 24, weight: .medium))
                            .foregroundColor(Colors.tone7)
                    Spacer()
                }.frame(width: geom.size.width, alignment: .center)
            }
        }
    }
}
