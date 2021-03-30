//
// Created by Semyon Tikhonenko on 2/27/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#include "LyricsPlayer.h"

LyricsPlayer::LyricsPlayer(const Lyrics *lyrics, int simultaneouslyLinesDisplayCount) :
lyrics(lyrics), 
simultaneouslyLinesDisplayCount(simultaneouslyLinesDisplayCount) {
    int linesCount = lyrics->getLinesCount();
    if (linesCount < simultaneouslyLinesDisplayCount) {
        simultaneouslyLinesDisplayCount = linesCount;
    }
}

double LyricsPlayer::getSeek() const {
    return seek;
}

void LyricsPlayer::setSeek(double seek) {
    this->seek = seek;
    updateLines();
}

void LyricsPlayer::updateLines() {
    int currentFirstLineIndexBefore = currentOrNextLineIndex;
    currentOrNextLineIndex = lyrics->getNextOrCurrentLineIndexBySeek(seek);
    if (currentOrNextLineIndex < 0 || currentOrNextLineIndex >= lyrics->getLinesCount()) {
        currentOrNextLineIndex = lyrics->getLinesCount() - 1;
    }

    if (currentFirstLineIndexBefore != currentOrNextLineIndex) {
        if (onLinesChanged) {
            onLinesChanged(this);
        }
    }

    updateSelection();
}

void LyricsPlayer::updateSelection() {
    Selection selection;
    if (seek > lyrics->getLastLine().getEndSeek()) {
        selection.lineSelection.charactersCount = lyrics->getLastLine().charactersCount;
        selection.lineSelection.lastCharacterSelectionPosition = 1.0;
        selection.lineIndex = 1;
    } else {
        selection.lineIndex = currentOrNextLineIndex == lyrics->getLinesCount() - 1 ? 1 : 0;
        int index = convertDisplayedLineIndexIntoGlobalIndex(selection.lineIndex);
        const auto& line = lyrics->getLineAt(index);
        selection.lineSelection = lyrics->getLineSelection(line, seek);
        assert(simultaneouslyLinesDisplayCount == 2 && "Hardcoded, pls fix");
    }

    if (selection != this->selection) {
        this->selection = selection;
        assert(getDisplayedLineAt(selection.lineIndex).size() >= selection.lineSelection.charactersCount);
        onSelectionChanged(this->selection);
    }
}

int LyricsPlayer::getDisplayLinesCount() const {
    return simultaneouslyLinesDisplayCount;
}

std::u32string_view LyricsPlayer::getDisplayedLineAt(int index) const {
    index = convertDisplayedLineIndexIntoGlobalIndex(index);
    return lyrics->getLineTextAt(index);
}

int LyricsPlayer::convertDisplayedLineIndexIntoGlobalIndex(int index) const {
    assert(index < getDisplayLinesCount());
    int firstDisplayedLineIndex = currentOrNextLineIndex;
    if (firstDisplayedLineIndex > lyrics->getLinesCount() - simultaneouslyLinesDisplayCount) {
        firstDisplayedLineIndex = lyrics->getLinesCount() - simultaneouslyLinesDisplayCount;
    }

    return firstDisplayedLineIndex + index;
}

bool LyricsPlayer::Selection::operator==(const LyricsPlayer::Selection &rhs) const {
    return lineSelection == rhs.lineSelection && lineIndex == rhs.lineIndex;
}

bool LyricsPlayer::Selection::operator!=(const LyricsPlayer::Selection &rhs) const {
    return !(rhs == *this);
}
