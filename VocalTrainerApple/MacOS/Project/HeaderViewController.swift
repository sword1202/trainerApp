//
// Created by Semyon Tykhonenko on 2019-07-28.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa
import Logic

class HeaderViewController : NSViewController, ConfigurableWithProjectController {
    private var projectController: ProjectControllerBridge!

    @IBOutlet private weak var lyricsButton: HeaderButton!
    @IBOutlet private weak var tracksButton: HeaderButton!
    @IBOutlet private weak var metronomeButton: HeaderButton!

    @IBOutlet private weak var playbackControlsView: NSView!
    @IBOutlet private weak var playButton: ToggleButton!
    @IBOutlet private weak var boundsButton: ToggleButton!
    @IBOutlet private weak var backwardButton: ToggleButton!
    @IBOutlet private weak var forwardButton: ToggleButton!
    @IBOutlet private weak var toBeginningButton: Button!

    func configure(projectController: ProjectControllerBridge) {
        self.projectController = projectController
        self.projectController.add(delegate: self)
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        updateButtonState(button: lyricsButton, value: projectController.lyricsVisible)
        updateButtonState(button: metronomeButton, value: projectController.metronomeEnabled)
        updateButtonState(button: tracksButton, value: projectController.tracksVisible)
        updateButtonState(button: playButton, value: projectController.isPlaying)
        updateButtonState(button: boundsButton, value: projectController.boundsSelectionEnabled)

        lyricsButton.handler = { [weak self] in
            self?.projectController.toggleLyricsVisibility()
        }

        metronomeButton.handler = { [weak self] in
            self?.projectController.toggleMetronomeEnabled()
        }

        tracksButton.handler = { [weak self] in
            self?.projectController.toggleTracksVisibility()
        }

        playbackControlsView.wantsLayer = true
        playbackControlsView.layer?.backgroundColor = NSColor(hex: 0xC2CFFE)!.cgColor
        playbackControlsView.layer?.cornerRadius = 22.5
        // For some reasons the shadow is displayed upside down, so revert it
        HeaderUiUtils.applyShadow(view: self.playbackControlsView, cornerRadius: 22.5, revert: true);

        playButton.handler = { [weak self] in
            self?.projectController.togglePlay()
        }

        boundsButton.handler = { [weak self] in
            self?.projectController.toggleBoundsSelectionEnabled()
        }
        
        forwardButton.handler = { [weak self] in
            self?.projectController.toggleRewind(withBackward: false)
        }

        backwardButton.handler = { [weak self] in
            self?.projectController.toggleRewind(withBackward: true)
        }
    }

    private func updateButtonState(button: ToggleButton, value: Bool) {
        button.state = value ? .on : .off
    }

    deinit {
        self.projectController.remove(delegate: self)
    }
}

extension HeaderViewController : ProjectControllerBridgeDelegate {
    public func projectController(didChangeLyricsVisibility showLyrics: Bool) {
        updateButtonState(button: lyricsButton, value: showLyrics)
    }

    public func projectController(didChangeTracksVisibility showTracks: Bool) {
        updateButtonState(button: tracksButton, value: showTracks)
    }

    public func projectController(didChangeMetronomeEnabled enabled: Bool) {
        updateButtonState(button: metronomeButton, value: enabled)
    }

    public func projectControllerUpdate(audioLevel: Double) {
    }

    public func projectControllerUpdate(seek: Double) {
    }

    public func projectControllerPlaybackDidStart() {
        self.playButton.state = .on
    }

    public func projectControllerPlaybackDidStop() {
        self.playButton.state = .off
    }

    public func projectController(didChangeHasLyrics hasLyrics: Bool) {
    }

    public func projectControllerUpdate(lyricsText: String) {
    }

    public func projectControllerUpdate(vocalPianoVolume: Float) {
    }

    public func projectControllerUpdate(instrumentalVolume: Float) {
    }

    public func projectControllerUpdate(vocalVolume: Float) {
    }

    public func projectControllerUpdate(inputSensitivity: Float) {
    }

    public func projectControllerUpdate(hasSaveIndicator: Bool) {
    }

    public func projectController(didChangeZoom: Float) {
    }

    public func projectController(didChangeRewindStatus running: Bool, isBackward: Bool) {
        let button = (isBackward ? backwardButton : forwardButton)!
        updateButtonState(button: button, value: running);
    }
}
