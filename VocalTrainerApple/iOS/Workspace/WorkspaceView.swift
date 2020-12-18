//
// Created by Semyon Tikhonenko on 12/17/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import Foundation
import SwiftUI
import GestureRecognizerClosures

private let baseZoomScaleFactor: CGFloat = 1.5

struct WorkspaceView : UIViewRepresentable {
    typealias UIViewType = UIView
    private let projectController = ProjectController.shared

    func makeUIView(context: Context) -> UIViewType {
        guard !SwiftUIUtils.isPreview() else {
            return UIView()
        }

        let view = WorkspaceDrawerView()
        view.onWorkspaceControllerChanged = { [unowned view] in
            projectController.setWorkspaceController(view.workspaceController());
        }

        var baseZoom = projectController.minZoom
        view.onPinch { pinch in
            if pinch.state == .began {
                baseZoom = projectController.zoom
            }

            let minZoom = projectController.minZoom
            let maxZoom = projectController.maxZoom
            let zoomScaleFactor = baseZoomScaleFactor / CGFloat(baseZoom - minZoom + 1.0)
            let scale = (pinch.scale - 1.0) / zoomScaleFactor + 1.0 + CGFloat(baseZoom - minZoom)
            let zoom = scale.cutToMatchClosedRange(min: CGFloat(minZoom), max: CGFloat(maxZoom))
            projectController.zoom = Float(zoom)
        }

        return view
    }

    func updateUIView(_ uiView: UIViewType, context: Context) {

    }
}
