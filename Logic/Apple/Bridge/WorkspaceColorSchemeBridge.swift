//
// Created by Semyon Tikhonenko on 7/27/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

private extension UInt32 {
    func rotated(angle: CGFloat) -> UInt32 {
        guard self != 0 else {
            return 0
        }

        return NativeColorUtils.rotateColorPalette(angle, color: self)
    }
}

public class WorkspaceColorSchemeBridge: NSObject {
    @objc public let gridColor: UInt32
    @objc public let accentGridColor: UInt32
    @objc public let pianoBorderColor: UInt32
    @objc public let pitchGraphColor: UInt32
    @objc public let pitchColor: UInt32
    @objc public let pianoSharpPitchColor: UInt32
    @objc public let reachedPitchColor: UInt32
    @objc public let missedPitchColor: UInt32
    @objc public let pianoSelectedPitchColor: UInt32
    @objc public let borderLineColor: UInt32
    @objc public let boundsColor: UInt32
    @objc public let playHeadColor: UInt32
    @objc public let instrumentalTrackColor: UInt32
    @objc public let pianoTrackColor: UInt32
    @objc public let pianoTrackShadowColor: UInt32
    @objc public let pianoTrackPitchesColor: UInt32
    @objc public let endingColor: UInt32

    @objc public init(
            gridColor: UInt32 = 0,
            accentGridColor: UInt32 = 0,
            pianoBorderColor: UInt32 = 0,
            pitchGraphColor: UInt32 = 0,
            pitchColor: UInt32 = 0,
            pianoSharpPitchColor: UInt32 = 0,
            reachedPitchColor: UInt32 = 0,
            missedPitchColor: UInt32 = 0,
            pianoSelectedPitchColor: UInt32 = 0,
            borderLineColor: UInt32 = 0,
            boundsColor: UInt32 = 0,
            playHeadColor: UInt32 = 0,
            instrumentalTrackColor: UInt32 = 0,
            pianoTrackColor: UInt32 = 0,
            pianoTrackShadowColor: UInt32 = 0,
            pianoTrackPitchesColor: UInt32 = 0,
            endingColor: UInt32 = 0) {
        self.gridColor = gridColor
        self.accentGridColor = accentGridColor
        self.pitchGraphColor = pitchGraphColor
        self.pitchColor = pitchColor
        self.pianoSharpPitchColor = pianoSharpPitchColor
        self.reachedPitchColor = reachedPitchColor
        self.missedPitchColor = missedPitchColor
        self.pianoSelectedPitchColor = pianoSelectedPitchColor
        self.borderLineColor = borderLineColor
        self.boundsColor = boundsColor
        self.playHeadColor = playHeadColor
        self.instrumentalTrackColor = instrumentalTrackColor
        self.pianoTrackColor = pianoTrackColor
        self.pianoTrackShadowColor = pianoTrackShadowColor
        self.pianoTrackPitchesColor = pianoTrackPitchesColor
        self.endingColor = endingColor
        self.pianoBorderColor = pianoBorderColor
        super.init()
    }

    public func rotated(angle: CGFloat) -> WorkspaceColorSchemeBridge {
        WorkspaceColorSchemeBridge(
                gridColor: gridColor.rotated(angle: angle),
                accentGridColor: accentGridColor.rotated(angle: angle),
                pianoBorderColor: pianoBorderColor.rotated(angle: angle),
                pitchGraphColor: pitchGraphColor.rotated(angle: angle),
                pitchColor: pitchColor.rotated(angle: angle),
                pianoSharpPitchColor: pianoSharpPitchColor.rotated(angle: angle),
                reachedPitchColor: reachedPitchColor.rotated(angle: angle),
                missedPitchColor: missedPitchColor.rotated(angle: angle),
                pianoSelectedPitchColor: pianoSelectedPitchColor.rotated(angle: angle),
                borderLineColor: borderLineColor.rotated(angle: angle),
                boundsColor: boundsColor.rotated(angle: angle),
                playHeadColor: playHeadColor.rotated(angle: angle),
                instrumentalTrackColor: instrumentalTrackColor.rotated(angle: angle),
                pianoTrackColor: pianoTrackColor.rotated(angle: angle),
                pianoTrackShadowColor: pianoTrackShadowColor.rotated(angle: angle),
                pianoTrackPitchesColor: pianoTrackPitchesColor.rotated(angle: angle),
                endingColor: endingColor.rotated(angle: angle))
    }
}
