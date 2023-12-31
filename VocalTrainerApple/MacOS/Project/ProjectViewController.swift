//
//  ProjectViewController.swift
//  VocalTrainerMacOS
//
//  Created by Semyon Tykhonenko on 7/27/19.
//  Copyright © 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa
import Logic

class ProjectViewController: NSViewController {
    @IBOutlet private weak var workspaceView: WorkspaceDrawerView!
    var projectController: ProjectControllerBridge!

    override func viewDidLoad() {
        super.viewDidLoad()
        if (!InitializationManager.isInitialized()) {
            InitializationManager.initializeApplicationIfNeed()
            projectController = ProjectControllerBridge()
            let mvxFilePath = Bundle.main.path(forResource: "drm", ofType: "mvx")
            projectController.setPlaybackSource(filePath: mvxFilePath)
        }
        self.workspaceView.onWorkspaceControllerChanged = { [unowned self] in
            projectController.setWorkspaceController(self.workspaceView.workspaceController())
        }
    }

    override func prepare(for segue: NSStoryboardSegue, sender: Any?) {
        if let controller = segue.destinationController as? ConfigurableWithProjectController {
            controller.configure(projectController: projectController)
        }
    }

    override var representedObject: Any? {
        didSet {
        }
    }
}

