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

        let size = CGSize(
                width: CGFloat(samples.count) * sampleWidth + CGFloat(samples.count - 1) * distanceBetweenSamples,
                height: sampleMaxHeight
        )

        return UIImage.draw(size: size) { ctx in
            var x: CGFloat = 0
            samples.forEach { sample in
                let height = CGFloat(sample) / CGFloat(max) * sampleMaxHeight
                let y: CGFloat = (sampleMaxHeight - height) / 2
                ctx.setFillColor(Colors.tone8.cgColor ?? UIColor.gray.cgColor)
                ctx.fill(
                        roundedRect: CGRect(x: x, y: y, width: sampleWidth, height: height),
                        cornerRadius: sampleWidth / 2.0)
                x += sampleWidth + distanceBetweenSamples
            }
        }
    }
}