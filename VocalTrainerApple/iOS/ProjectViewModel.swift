//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import Combine

class ProjectViewModel : ObservableObject {
    init() {
        if (!SwiftUIUtils.isPreview()) {
            InitializationManager.initializeApplicationIfNeed()
            ProjectController.shared.togglePlay()
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
