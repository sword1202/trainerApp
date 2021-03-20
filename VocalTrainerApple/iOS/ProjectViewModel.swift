//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import Combine

class ProjectViewModel : ObservableObject {
    @Published private(set) var isMetronomeEnabled = false
    @Published var lyricsLines: [String] = []
    @Published var lyricsSelection = LyricsSelection(characterIndex: 0, position: 0, lineIndex: 0)
    @Published var playbackSections: [PlaybackSection] = []

    private var disableProgressUpdate = false
    @Published var progress: CGFloat = 0 {
        didSet {
            if (oldValue != progress && !disableProgressUpdate) {
                projectController.setPlaybackProgress(Double(progress))
            }
        }
    }
    private lazy var projectController = ProjectController.shared

    init() {
        if !SwiftUIUtils.isPreview() {
            isMetronomeEnabled = projectController.metronomeEnabled
            projectController.add(delegate: self)
            playbackSections = projectController.lyricsSections.map {
                let name = Strings.from(sectionType: $0.type).localized + " " + String($0.number)
                let position = CGFloat(projectController.convertSeek(toPlaybackProgress: $0.seek))
                return PlaybackSection(name: name, position: position)
            }
        } else {
            playbackSections = [
                .init(name: "Verse 1", position: 0.2),
                .init(name: "Chorus 1", position: 0.25),
                .init(name: "Verse 2", position: 0.35),
                .init(name: "Chorus 2", position: 0.5),
                .init(name: "Bridge 1", position: 0.6),
                .init(name: "Chorus 3", position: 0.8),
            ]
        }
    }

    deinit {
        if !SwiftUIUtils.isPreview() {
            projectController.remove(delegate: self)
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

    public func projectControllerUpdate(currentLyricsLines: [String]) {
        self.lyricsLines = currentLyricsLines;
    }

    func projectControllerUpdateLyricsSelection(
            selectedCharactersCount: Int,
            lastCharacterSelectionPosition: CGFloat,
            lineIndex: Int
    ) {
        lyricsSelection = LyricsSelection(
                characterIndex: selectedCharactersCount == 0 ? 0 : selectedCharactersCount - 1,
                position: lastCharacterSelectionPosition,
                lineIndex: lineIndex)
    }

    func projectControllerUpdate(seek: Double) {
        disableProgressUpdate = true
        progress = CGFloat(projectController.convertSeek(toPlaybackProgress: seek))
        disableProgressUpdate = false
    }
}
