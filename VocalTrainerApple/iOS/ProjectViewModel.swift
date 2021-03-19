//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import Combine

class ProjectViewModel : ObservableObject {
    @Published private(set) var isMetronomeEnabled = false
    @Published var lyricsLines: [String] = []
    @Published var lyricsSelection = LyricsSelection(characterIndex: 0, position: 0, lineIndex: 0)
    private lazy var projectController = ProjectController.shared

    init() {
        if !SwiftUIUtils.isPreview() {
            isMetronomeEnabled = projectController.metronomeEnabled
            projectController.add(delegate: self)
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

        if (lineIndex != 0) {
            print(lyricsSelection)
        }
    }
}
