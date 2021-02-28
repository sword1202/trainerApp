//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import UIKit

private let progressSectionHeight: CGFloat = 12
private let progressSectionWidth: CGFloat = 1
private let progressLineHeight: CGFloat = 8
private let progressDotRadius: CGFloat = 7

struct PlaybackSection {
    let name: String
    let position: CGFloat

    init(name: String, position: CGFloat) {
        assert(position <= 0 && position >= 1)
        self.name = name
        self.position = position
    }
}

class PlaybackSlider : UIView {
    var progress: CGFloat = 0 {
        didSet {
            updateProgress()
        }
    }

    var sections = [PlaybackSection]() {
        didSet {
            updateProgress()
        }
    }

    override var frame: CGRect {
        didSet {
            postInit()
        }
    }

    private let progressBackgroundView = UIImageView()

    override init(frame: CGRect) {
        super.init(frame: frame)
        postInit()
    }

    required init?(coder: NSCoder) {
        super.init(coder: coder)
        postInit()
    }

    private func postInit() {
        progressBackgroundView.frame = CGRect(origin: CGPoint(),
                size: CGSize(width: self.frame.size.width, height: progressSectionHeight))
        updateProgress()
    }

    private func updateProgress() {
        let rect = CGRect(origin: .zero, size: progressBackgroundView.frame.size)
        progressBackgroundView.image = UIImage.draw(size: rect.size) { ctx in
            let lineYOffset = progressSectionHeight - progressLineHeight
            ctx.setFillColor(Colors.tone3.cgColor!)
            let lineRect = rect.insetBy(top: lineYOffset)
            ctx.fill(roundedRect: lineRect, cornerRadius: progressLineHeight / 2)
            let progressRect = lineRect.insetBy(dx: progressDotRadius, dy: 0)
            ctx.setFillColor(Colors.tone2.cgColor!)
            ctx.fill(progressRect)

            ctx.setFillColor(Colors.tone4.cgColor!)
            for section in sections {
                let sectionRect = CGRect(
                        origin: .init(x: progressRect.width * section.position - progressSectionWidth / 2.0 +
                                progressDotRadius, y: 0),
                        size: CGSize(width: progressSectionWidth, height: progressSectionHeight))
                ctx.fill(sectionRect)
            }
        }
    }
}
