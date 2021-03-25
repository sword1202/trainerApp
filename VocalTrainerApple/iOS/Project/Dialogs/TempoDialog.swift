//
// Created by Semyon Tikhonenko on 3/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct TempoDialog : View {
    @ObservedObject private var viewModel: TempoViewModel
    @Binding var isShown: Bool

    init(viewModel: TempoViewModel, isShown: Binding<Bool>) {
        self.viewModel = viewModel
        _isShown = isShown
    }

    var body: some View {
        BottomDialog(title: Strings.key.localized, titleIcon:"TonalityIcon", isShown: $isShown) {
            VStack(alignment: .center, spacing: 0) {
                PlusMinusDialogPanel(
                        topText: $viewModel.bpmLabel,
                        middleText: $viewModel.factorString,
                        bottomText: $viewModel.originalBpm,
                        plus: {
                            viewModel.incrementFactor()
                        },
                        minus: {
                            viewModel.decrementFactor()
                        }
                )
            }.frame(maxWidth: .infinity, maxHeight: 244).background(Color.white)
        }
    }
}
