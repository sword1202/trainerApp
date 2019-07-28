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
        InitializationManager.initializeApplicationIfNeed()
        projectController = ProjectControllerBridge(delegate: self)
        self.workspaceView.configure(projectController)
    }

    override var representedObject: Any? {
        didSet {
        }
    }
}

extension ProjectViewController : ProjectControllerBridgeDelegate {
    func projectControllerUpdate(audioLevel: Double) {

    }

    func projectControllerUpdate(seek: Double) {

    }

    func projectControllerPlaybackDidStart() {

    }

    func projectControllerPlaybackDidStop() {

    }

    func projectControllerUpdate(lyricsLine: String) {

    }

    func projectControllerUpdate(vocalPianoVolume: Float) {

    }

    func projectControllerUpdate(instrumentalVolume: Float) {

    }

    func projectControllerUpdate(vocalVolume: Float) {

    }

    func projectControllerUpdate(inputSensitivity: Float) {

    }

    func projectControllerUpdate(hasSaveIndicator: Bool) {

    }

    func projectController(didChangeHasLyrics hasLyrics: Bool) {

    }

    func projectController(didChangeShowLyrics showLyrics: Bool) {

    }

    func projectController(didChangeMetronomeEnabled enabled: Bool) {

    }
}

