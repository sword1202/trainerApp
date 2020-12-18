//
// Created by Semyon Tikhonenko on 12/17/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import SwiftUI

struct WorkspaceView : UIViewRepresentable {
    typealias UIViewType = UIView

    func makeUIView(context: Context) -> UIViewType {
        guard !SwiftUIUtils.isPreview() else {
            return UIView()
        }

        let view = WorkspaceDrawerView()
        view.onWorkspaceControllerChanged = { [unowned view] in
            ProjectController.shared.setWorkspaceController(view.workspaceController());
        }
        return view
    }

    func updateUIView(_ uiView: UIViewType, context: Context) {

    }
}
