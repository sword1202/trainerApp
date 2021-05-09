//
// Created by Semyon Tikhonenko on 5/9/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct SongCompletionDialog : View {
    @Binding var isShown: Bool

    var body: some View {
        BottomDialog(title: Strings.listenToTheResult.localized, titleIcon:"Headphones", isShown: $isShown) {
            VStack {
                Image("SongCompletionDialogIcon")
            }
        }
    }
}