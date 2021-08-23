//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import Combine
import AVFoundation
import SwiftUI

class ProjectViewModel : ObservableObject {
    private let audioEngine = AVAudioEngine()

    @Published var isMetronomeEnabled = false {
        didSet {
            projectController.metronomeEnabled = isMetronomeEnabled
        }
    }
    @Published var isLyricsVisible = false {
        didSet {
            projectController.lyricsVisible = isLyricsVisible
        }
    }
    @Published var lyricsLines: [String] = []
    @Published var lyricsSelection = LyricsSelection(characterIndex: 0, position: 0, lineIndex: 0)
    @Published var playbackSections: [PlaybackSection] = []
    @Published var title: String = ""
    @Published var playbackCurrentTime: String = ""
    @Published var playbackEndTime: String = ""
    @Published private(set) var retrySeconds = 5
    @Published private(set) var isPlaying: Bool = false
    @Published var showSongCompletionFlow: Bool = false
    private(set) var songCompletionFlow: SongCompletionFlowBridge?

    @Published private(set) var tone: [Color] = []

    private let timeFormatter = DateFormatter()

    private var disableProgressUpdate = false
    @Published var progress: CGFloat = 0 {
        didSet {
            if oldValue != progress && !disableProgressUpdate {
                projectController.setPlaybackProgress(Double(progress))
            }
        }
    }

    var originalTonality : String {
        get {
            projectController.originalTonality.toString()
        }
    }

    private(set) lazy var projectController = ProjectController()

    private func updatePlaybackEndTime() {
        playbackEndTime = timeFormatter.string(from: Date(timeIntervalSince1970: projectController.endSeek))
    }

    init(filePath: String?) {
        projectController.add(delegate: self)
        let mvxFilePath = filePath ?? Bundle.main.path(forResource: "drm", ofType: "mvx")
        projectController.setPlaybackSource(filePath: mvxFilePath)

        isMetronomeEnabled = projectController.metronomeEnabled
        updatePlaybackSections()
        title = projectController.artistName + " - " + projectController.songTitle
        timeFormatter.dateFormat = "m:ss"
        updatePlaybackEndTime()

//        audioEngine.connect(
//                audioEngine.inputNode,
//                to: audioEngine.outputNode,
//                format: audioEngine.inputNode.inputFormat(forBus: 0))
    }

    private func updatePlaybackSections() {
        let lyricsSections: [LyricsSection] = projectController.lyricsSections
        playbackSections = lyricsSections.map {
            let type = $0.type
            let name = Strings.from(sectionType: type).localized
            let position = CGFloat(projectController.convertSeek(toPlaybackProgress: $0.seek))
            return PlaybackSection(name: name, position: position)
        }
    }

    deinit {
        projectController.remove(delegate: self)
    }

    func willBecomeInactive() {
        projectController.stop()
    }
    
    func didBecomeActive() {

    }
    
    func didTapPlayButton() {
        projectController.togglePlay()
    }

    func didTapRetry() {
        projectController.rewindBack(bySeconds: Double(retrySeconds))
    }

    func didTapBoundsSelection(showBoundsSelectionDialog: inout Bool) {
        showBoundsSelectionDialog = !projectController.hasPlaybackBounds()
        if !showBoundsSelectionDialog {
            projectController.clearPlaybackBounds()
        }
    }

    func isRecording() -> Bool {
        projectController.isRecording
    }

    func createTonalityViewModel() -> TonalityViewModel {
        TonalityViewModel(projectController: projectController)
    }

    func createTempoViewModel() -> TempoViewModel {
        TempoViewModel(projectController: projectController)
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
            lastCharacterSelectionPosition: Float,
            lineIndex: Int
    ) {
        lyricsSelection = LyricsSelection(
                characterIndex: selectedCharactersCount == 0 ? 0 : selectedCharactersCount - 1,
                position: CGFloat(lastCharacterSelectionPosition),
                lineIndex: lineIndex)
    }

    func projectControllerUpdate(seek: Double) {
        disableProgressUpdate = true
        progress = CGFloat(projectController.convertSeek(toPlaybackProgress: seek))
        disableProgressUpdate = false
        playbackCurrentTime = timeFormatter.string(from: Date(timeIntervalSince1970: seek))
    }

    func projectControllerUpdate(lyricsVisibility: Bool) {
        isLyricsVisible = lyricsVisibility;
    }

    func projectControllerUpdate(endSeek: Double) {
        updatePlaybackEndTime()
        updatePlaybackSections()
    }

    func projectControllerPlaybackDidStart() {
        //try! audioEngine.start()
        isPlaying = true
    }

    func projectControllerPlaybackDidStop() {
        //audioEngine.stop()
        isPlaying = false
    }

    func projectControllerPlaybackDidComplete(flow: SongCompletionFlowBridge) {
        songCompletionFlow = flow
        showSongCompletionFlow = true
    }

    func projectControllerPlaybackSourceDidChange() {
        tone = [
            Colors.tone1,
            Colors.tone2,
            Colors.tone3,
            Colors.tone4,
            Colors.tone5,
            Colors.tone6,
            Colors.tone7,
        ]
        if isRecording() {
            tone = tone.map { $0.rotate(angle: Colors.recordingPaletteRotation) }
        }
        projectController.setWorkspaceColorScheme(Colors.getWorkspaceScheme(isRecording: isRecording()))
    }
}
