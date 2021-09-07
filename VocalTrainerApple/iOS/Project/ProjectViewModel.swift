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
    private(set) var songCompletionFlow: SongCompletionFlowBridge? = nil
    @Published var showListenScreen = false
    @Published private(set) var recording: PlaybackSource? = nil
    @Published private(set) var recordingTimeLabel: String = ""
    @Published private(set) var isRecording: Bool = false
    private var source: PlaybackSource!

    @Published private(set) var tone: [Color] = [
        Colors.tone1,
        Colors.tone2,
        Colors.tone3,
        Colors.tone4,
        Colors.tone5,
        Colors.tone6,
        Colors.tone7
    ]

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

    func configure(source: PlaybackSource) {
        guard self.source == nil else {
            return
        }

        self.source = source
        projectController.add(delegate: self)
        projectController.setPlaybackSource(source)
        isRecording = projectController.isRecording

        isMetronomeEnabled = projectController.metronomeEnabled
        updatePlaybackSections()
        title = projectController.artistName + " - " + projectController.songTitle
        timeFormatter.dateFormat = "m:ss"
        updatePlaybackEndTime()

        if isRecording {
            let recordingTime = source.creationTime
            let formatter = RelativeDateTimeFormatter()
            formatter.unitsStyle = .full
            recordingTimeLabel = Strings.recordedAgoLabel.localized + " " +
                    formatter.localizedString(fromTimeInterval: Date().timeIntervalSince1970 - recordingTime)
            if recordingTimeLabel.contains(" 0 ") {
                recordingTimeLabel = Strings.recordedAgoLabel.localized + " " + Strings.justNow.localized
            }
        }

        projectController.setWorkspaceColorScheme(Colors.getWorkspaceScheme(isRecording: false))
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
        if source.isTempRecording || !projectController.isRecording {
            songCompletionFlow = flow
            showSongCompletionFlow = true
        }
    }

    func projectControllerStartListeningToRecording(recording: PlaybackSource) {
        self.recording = recording
        showListenScreen = true
    }
}
