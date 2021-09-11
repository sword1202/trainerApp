//
// Created by Semyon Tikhonenko on 6/19/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct RecordingView : View {
    private let recordingBackground: UIImage
    private let title: String
    private let artistName: String
    private let date: String
    private let time: String

    init(recordingBackground: UIImage, title: String, artistName: String, date: Date) {
        self.recordingBackground = recordingBackground
        self.title = title
        self.artistName = artistName
        let formatter = DateFormatter()
        formatter.dateFormat = "dd MMM yyy"
        self.date = formatter.string(from: date)
        formatter.dateFormat = "HH:mm"
        time = formatter.string(from: date)
    }

    var body: some View {
        VStack(spacing: 0) {
            ZStack {
                HStack {
                    Spacer()
                    Image(uiImage: recordingBackground)
                    Spacer()
                }.frame(alignment: .center).frame(maxHeight: .infinity)
                HStack {
                    VStack(alignment: .leading, spacing: 0) {
                        Text(title)
                                .foregroundColor(Colors.tone5)
                                .font(Font.system(size: 18, weight: .medium))
                        Spacer()
                        Text(artistName)
                                .foregroundColor(Colors.secondaryTextColor)
                                .font(Font.system(size: 18, weight: .medium))
                    }.frame(maxHeight: .infinity)
                    Spacer()
                    VStack(alignment: .leading, spacing: 0) {
                        Text(date)
                                .foregroundColor(Colors.secondaryTextColor)
                                .font(Font.system(size: 18, weight: .medium))
                        Spacer()
                        Text(time)
                                .foregroundColor(Colors.secondaryTextColor)
                                .font(Font.system(size: 18, weight: .medium))
                    }.frame(maxHeight: .infinity)
                }.frame(maxWidth: .infinity)
                        .padding(.leading, 16)
                        .padding(.trailing, 16)
                        .padding(.top, 22)
                        .padding(.bottom, 22)
            }
            Divider()
        }
    }
}

struct RecordingsView : View {
    @StateObject private var viewModel = RecordingsViewModel()

    var body: some View {
        GeometryReader { geom in
            // Fill safe area with colors
            Spacer().frame(maxWidth: .infinity).background(Colors.tone2).ignoresSafeArea()
            Spacer().frame(maxWidth: .infinity).background(Color.white)

            // Main content
            VStack(spacing: 0) {
                HStack {
                    Text(Strings.recordings.localized)
                            .font(Font.system(size: 34))
                            .foregroundColor(Color.white)
                            .padding(.leading, 16)
                    Spacer()
                    NavigationLink(
                            destination: NavigationLazyView(ProjectView(source: PlaybackSource.training()).navigationBarHidden(true))
                    ) {
                        Image("WhitePlusButton")
                    }.padding(.trailing, 18)
                }.frame(height: 72, alignment: .center).background(Colors.tone2)

                if (viewModel.recordingsCount > 0) {
                    ScrollView {
                        LazyVStack(spacing: 0) {
                            ForEach(0..<viewModel.recordingsCount) { index in
                                let recording = viewModel.getRecording(at: index)
                                NavigationLink(
                                        destination: NavigationLazyView(
                                                ProjectView(source: PlaybackSource(filePath: recording.filePath)).navigationBarHidden(true))
                                ) {
                                    RecordingView(
                                            recordingBackground: viewModel.getPreviewSamplesImage(
                                                    at: index, width: geom.size.width - 30),
                                            title: recording.songTitle,
                                            artistName: recording.originalArtistName,
                                            date: Date(timeIntervalSince1970: recording.date)
                                    ).frame(minHeight: 100).frame(maxWidth: .infinity).onDelete {
                                        viewModel.deleteRecording(at: index)
                                    }
                                }
                            }
                        }
                    }
                } else {
                    VStack(spacing: 16) {
                        Spacer()
                        Image("NoRecordings")
                        Text(Strings.noRecordingsMessage.localized)
                                .frame(width: 180)
                                .multilineTextAlignment(.center)
                                .font(Font.system(size: 24, weight: .medium))
                                .foregroundColor(Colors.tone7)
                        Spacer()
                    }.frame(width: geom.size.width, alignment: .center)
                }
            }
        }
    }
}
