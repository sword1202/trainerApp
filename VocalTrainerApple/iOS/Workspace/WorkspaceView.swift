//
// Created by Semyon Tikhonenko on 12/17/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import SwiftUI

struct WorkspaceView : UIViewRepresentable {
    typealias UIViewType = WorkspaceDrawerView

    func makeUIView(context: Context) -> UIViewType {
        WorkspaceDrawerView()
    }

    func updateUIView(_ uiView: UIViewType, context: Context) {

    }
}
