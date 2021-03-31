//
// Created by Semyon Tikhonenko on 3/30/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

class BoundsSelectionViewModel : ObservableObject {
    private let projectController = ProjectController.shared
    @Published var lineSelectionStates: [Bool] {
        didSet {
            isLoopItAvailable = lineSelectionStates.contains(true)
        }
    }
    @Published private(set) var isLoopItAvailable = false

    init() {
        lineSelectionStates = Array(repeating: false, count: projectController.linesCount)
    }

    func getLine(index: Int) -> String {
        projectController.getLine(index: index)
    }

    func getSectionsCount() -> Int {
        projectController.lyricsSections.count
    }

    func getSectionFirstLineIndex(sectionIndex: Int) -> Int {
        projectController.lyricsSections[sectionIndex].firstLineIndex
    }

    func getLinesCount(inSection index: Int) -> Int {
        projectController.lyricsSections[index].linesCount
    }

    func getSection(at index: Int) -> String {
        Strings.from(sectionType: projectController.lyricsSections[index].type).localized
    }

    func didTapOnLine(lineIndex: Int) {
        if lineSelectionStates[lineIndex] {
            if (0..<lineSelectionStates.count).contains(where: { $0 != lineIndex && lineSelectionStates[$0] }) {
                lineSelectionStates.fill(value: false)
                lineSelectionStates[lineIndex] = true
            } else {
                lineSelectionStates[lineIndex] = false
            }
        } else {
            if let indexOfSelection = lineSelectionStates.indexOf(true) {
                if lineSelectionStates.count(element: true) == 1 {
                    let range = minmax(lineIndex, indexOfSelection)
                    for i in range.0...range.1 {
                        lineSelectionStates[i] = true
                    }
                } else {
                    lineSelectionStates.fill(value: false)
                    lineSelectionStates[lineIndex] = true
                }
            } else {
                lineSelectionStates[lineIndex] = true
            }
        }
    }

    func didTapLoopIt() {
        guard let firstSelectedLineIndex = lineSelectionStates.indexOf(true),
              let lastSelectedLineIndex = lineSelectionStates.lastIndexOf(true) else {
            return
        }

        projectController.loopLines(firstLineIndex: firstSelectedLineIndex, lastLineIndex: lastSelectedLineIndex)
    }
}