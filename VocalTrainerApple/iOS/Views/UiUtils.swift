//
// Created by Semyon Tikhonenko on 5/16/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import UIKit

private let sampleWidth: CGFloat = 3
private let distanceBetweenSamples: CGFloat = 3
private let sampleMaxHeight: CGFloat = 72
private let sampleMinHeight: CGFloat = 8


struct UiUtils {
    static func drawRecordingPreviewTrack(samples: [Float]) -> UIImage {
        guard var max = samples.max() else {
            return UIImage()
        }

        if (max < 0.5) {
            max = 0.5
        }

        let heights: [CGFloat] = samples.map { sample in
            CGFloat(CGFloat(abs(sample)) / CGFloat(max) * (sampleMaxHeight - sampleMinHeight) + sampleMinHeight)
        }

        let size = CGSize(
                width: CGFloat(samples.count) * sampleWidth + CGFloat(samples.count - 1) * distanceBetweenSamples,
                height: heights.max() ?? sampleMaxHeight
        )

        return UIImage.draw(size: size) { ctx in
            ctx.setFillColor(Colors.recordingTrack.cgColor ?? UIColor.gray.cgColor)
            var x: CGFloat = 0
            heights.forEach { height in
                let y: CGFloat = (size.height - height) / 2
                ctx.fill(
                        roundedRect: CGRect(x: x, y: y, width: sampleWidth, height: height),
                        cornerRadius: sampleWidth / 2.0)
                x += sampleWidth + distanceBetweenSamples
            }
        }
    }
}