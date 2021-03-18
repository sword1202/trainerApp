//
// Created by Semyon Tikhonenko on 3/15/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct LyricsView : View {
    @Binding var lines: [String]

    var body: some View {
        VStack(spacing: 8) {
            ForEach(0 ..< lines.count, id: \.self) {
                Text(lines[$0]).font(.system(size: 18, weight: .semibold)).foregroundColor(Colors.tone1)
            }
        }.frame(minWidth: 0, maxWidth: .infinity, minHeight: 0, maxHeight: .infinity).padding(16)
    }
}
