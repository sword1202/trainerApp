//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI
import SwiftHEXColors
import Logic

class Colors : NSObject {
    static let recordingPaletteRotation: CGFloat = 100

    static let tone1 = Color(hex: 0xC5CDFC, alpha: 1.0)
    static let tone2 = Color(hex: 0x353368, alpha: 1.0)
    static let tone3 = Color(hex: 0x615F97, alpha: 1.0)
    static let tone4 = Color(hex: 0x514F7D, alpha: 1.0)
    static let tone5 = Color(hex: 0x24232D, alpha: 1.0)
    static let tone6 = Color(hex: 0x5452AD, alpha: 1.0)
    static let tone7 = Color(hex: 0xAEAEB3, alpha: 1.0)

    static func getWorkspaceScheme(isRecording: Bool) -> WorkspaceColorSchemeBridge {
        let result = WorkspaceColorSchemeBridge(
                gridColor: 0x8B89B633,
                accentGridColor: 0x8B89B680,
                pianoBorderColor: 0x9A98D07f,
                pitchColor: 0x6E7EC5FF,
                pianoSharpPitchColor: 0x9A98D0ff,
                pianoSelectedPitchColor: 0x615F97ff,
                borderLineColor: 0x8B89B6CC,
                boundsColor: 0xC4CDFDFF,
                playHeadColor: 0x24232DFF,
                instrumentalTrackColor: 0x9798B5FF,
                pianoTrackColor: 0xffffffff,
                pianoTrackShadowColor: 0xDDDBEE99,
                pianoTrackPitchesColor: 0x514E64FF,
                endingColor: 0xFF5E85FF)
        return isRecording ? result.rotated(angle: recordingPaletteRotation) : result
    }

    static let recordingTrack = Color(hex: 0xC4C4C4, alpha: 1.0)
    static let recordingBackgroundTrack = Color(hex: 0xC7C7C7, alpha: 0.2)
    static let fadedButton = Color(hex: 0xF5F5F5, alpha: 1.0)
    static let alternativeButton = Color(hex: 0x2D2D2D, alpha: 1.0)
    static let warningColor = Color(hex: 0xEA3D3D, alpha: 1.0)
    static let headerBackground = Color(hex: 0xF9F9F9, alpha: 0.94)
    static let buttonTone1 = Color(hex: 0x767680, alpha: 0.12)
    static let divider = Color(hex: 0xEFEFEF, alpha: 1.0)
    static let secondaryTextColor = Color(hex: 0x767676, alpha: 1.0)
    static let recordedAgoLabelBackground = Color(hex: 0x202020, alpha: 1.0)
}
