//
// Created by Semyon Tikhonenko on 3/25/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct PlusMinusDialogPanel : View {
    @Binding var topText: String
    @Binding var middleText: String
    @Binding var bottomText: String
    let plus: () -> Void
    let minus: () -> Void

    var body: some View {
        VStack(alignment: .center, spacing: 0) {
            Text(topText)
                    .foregroundColor(Colors.tone5)
                    .padding(.bottom, 12)
                    .font(Font.system(size: 26, weight: .semibold))
            HStack(spacing: 22) {
                CircleButton(icon: "minus").onTapGesture(perform: minus)
                Text(middleText)
                        .foregroundColor(Colors.tone5)
                        .font(Font.system(size: 50, weight: .semibold))
                        .frame(width: 164)
                CircleButton(icon: "plus").onTapGesture(perform: plus)
            }
            Text(bottomText)
                    .foregroundColor(Colors.tone5)
                    .font(Font.system(size: 16, weight: .semibold))
                    .padding(.top, 24)
        }.frame(maxWidth: .infinity)
    }
}
