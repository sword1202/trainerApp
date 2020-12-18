//
// Created by Semyon Tikhonenko on 12/17/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import SwiftUI

struct WorkspaceView : UIViewRepresentable {
    typealias UIViewType = WorkspaceDrawerView

    func makeUIView(context: Context) -> UIViewType {
        let view = WorkspaceDrawerView()
        view.onWorkspaceControllerChanged = { [unowned view] in
            ProjectController.shared.setWorkspaceController(view.workspaceController());
            ProjectController.shared.togglePlay()
        }
        return view
    }

    func updateUIView(_ uiView: UIViewType, context: Context) {

    }
}
