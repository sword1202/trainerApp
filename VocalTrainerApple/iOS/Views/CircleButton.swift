//
// Created by Semyon Tikhonenko on 3/24/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct CircleButton : View {
    private let icon: String

    init(icon: String) {
        self.icon = icon
    }

    var body: some View {
        ZStack {
            Circle().fill(Colors.buttonTone1)
            Image(icon)
        }.frame(width: 30, height: 30)
    }
}
