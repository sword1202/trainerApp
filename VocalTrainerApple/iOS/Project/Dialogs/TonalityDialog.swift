//
// Created by Semyon Tikhonenko on 3/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct TonalityDialog : View {
    @ObservedObject private var viewModel: TonalityViewModel
    @Binding var isShown: Bool

    init(viewModel: TonalityViewModel, isShown: Binding<Bool>) {
        self.viewModel = viewModel
        _isShown = isShown
    }

    var body: some View {
        BottomDialog(title: Strings.key.localized, titleIcon:"TonalityIcon", isShown: $isShown) {
            VStack(alignment: .center, spacing: 0) {
                let shift = viewModel.shift
                Text(shift <= 0 ? shift.description : "+" + shift.description)
                        .foregroundColor(Colors.tone5)
                        .padding(.bottom, 12)
                        .font(Font.system(size: 26, weight: .semibold))
                HStack(spacing: 24) {
                    CircleButton(icon: "minus").onTapGesture {
                        viewModel.shift -= 1
                    }
                    Text(viewModel.tonality)
                            .foregroundColor(Colors.tone5)
                            .font(Font.system(size: 50, weight: .semibold))
                    CircleButton(icon: "plus").onTapGesture {
                        viewModel.shift += 1
                    }
                }

                Text(Strings.originalTonalityLabel.localized + " " + viewModel.originalTonality)
                        .foregroundColor(Colors.tone5)
                        .font(Font.system(size: 16, weight: .semibold))
                        .padding(.top, 24)
            }.frame(maxWidth: .infinity, maxHeight: 244).background(Color.white)
        }
    }
}
