//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import Combine

class ProjectViewModel : ObservableObject {
    @Published private(set) var isMetronomeEnabled = false
    private lazy var projectController = ProjectController.shared

    init() {
        if !SwiftUIUtils.isPreview() {
            isMetronomeEnabled = projectController.metronomeEnabled
        }
    }

    func willBecomeInactive() {
        ProjectController.shared.stop()
    }
    
    func didBecomeActive() {

    }
    
    func didTapPlayButton() {
        ProjectController.shared.togglePlay()
    }
}

extension ProjectViewModel : ProjectControllerBridgeDelegate {
    public func projectController(didChangeMetronomeEnabled enabled: Bool) {
        self.isMetronomeEnabled = enabled
    }
}
