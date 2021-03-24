//
// Created by Semyon Tikhonenko on 3/15/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI
import UIKit

private let selectionColor = Colors.tone1
private let notSelectedTextColor = Color.white

struct LyricsView : View {
    @Binding var lines: [String]
    @Binding var lyricsSelection: LyricsSelection

    private let uiFont = UIFont.systemFont(ofSize: 18, weight: .semibold)
    private let font = Font.system(size: 18, weight: .semibold)

    func createSelectionGradient(text: String) -> LinearGradient {
        let indexOfSelectedCharacter = text.index(text.startIndex,
                offsetBy: lyricsSelection.characterIndex)
        let ch = text[indexOfSelectedCharacter]
        let chWidth = uiFont.width(text: ch.description)

        let indexAfterSelectedCharacter = text.index(after: indexOfSelectedCharacter)
        let sizeIncludingCharacter = uiFont.width(text: String(text[..<indexAfterSelectedCharacter]))

        let chPosition = sizeIncludingCharacter - chWidth
        let selectionPosition = chPosition + chWidth * lyricsSelection.position
        let allTextWidth = uiFont.width(text: text)
        let relativeSelectionPosition = selectionPosition / allTextWidth

        return LinearGradient(gradient: Gradient(stops: [
            .init(color: selectionColor, location: 0),
            .init(color: selectionColor, location: relativeSelectionPosition),
            .init(color: notSelectedTextColor, location: relativeSelectionPosition),
            .init(color: notSelectedTextColor, location: 1)
        ]), startPoint: .leading, endPoint: .trailing)
    }

    var body: some View {
        VStack(spacing: 8) {
            ForEach(0 ..< lines.count, id: \.self) {
                let lineIndex = $0
                let line = lines[lineIndex]
                Text(line).font(font)
                        .foregroundColor(lyricsSelection.lineIndex == 0 ? notSelectedTextColor : selectionColor)
                        .if(lineIndex == lyricsSelection.lineIndex)
                        {
                            $0.foreground(createSelectionGradient(text: line))
                        }
            }
        }.frame(minWidth: 0, maxWidth: .infinity, minHeight: 0, maxHeight: .infinity).padding(16)
    }
}
