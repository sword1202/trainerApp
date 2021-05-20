//
// Created by Semyon Tikhonenko on 4/5/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct BigButton : View {
    private let text: String
    private let action: () -> Void
    private let textColor: Color
    private let backgroundColor: Color

    init(text: String,
         backgroundColor: Color = Colors.tone6,
         textColor: Color = Color.white,
         action: @escaping () -> ())
    {
        self.text = text
        self.action = action
        self.backgroundColor = backgroundColor
        self.textColor = textColor
    }

    var body: some View {
        Button(action: action) {
            ZStack {
                RoundedRectangle(cornerRadius: 8).fill(backgroundColor).frame(height: 50)
                Text(text).foregroundColor(textColor).font(Font.system(size: 17, weight: .semibold))
            }
        }
    }
}

extension BigButton {
    func moveToBottom(screenGeometry: GeometryProxy) -> some View {
        self.frame(maxWidth: .infinity)
                .padding(.leading, 16).padding(.trailing, 16)
                .padding(.bottom, max(screenGeometry.safeAreaInsets.bottom + 2, 16))
    }
}
