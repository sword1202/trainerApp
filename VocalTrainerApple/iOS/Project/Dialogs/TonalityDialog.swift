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
            ZStack {
                PlusMinusDialogPanel(
                        topText: $viewModel.shiftString,
                        middleText: $viewModel.tonality,
                        bottomText: $viewModel.originalTonalityLabel,
                        plus: {
                            viewModel.shift += 1
                        },
                        minus: {
                            viewModel.shift -= 1
                        }
                )
            }.frame(maxWidth: .infinity, maxHeight: 244).background(Color.white)
        }
    }
}
