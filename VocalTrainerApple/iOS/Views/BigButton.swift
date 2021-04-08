//
// Created by Semyon Tikhonenko on 4/5/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct BigButton : View {
    private let text: String
    private let action: () -> Void

    init(text: String, action: @escaping () -> ()) {
        self.text = text
        self.action = action
    }

    var body: some View {
        Button(action: action) {
            ZStack {
                RoundedRectangle(cornerRadius: 8).fill(Colors.tone6).frame(height: 50)
                Text(text).foregroundColor(Color.white).font(Font.system(size: 17, weight: .semibold))
            }
        }
    }
}
