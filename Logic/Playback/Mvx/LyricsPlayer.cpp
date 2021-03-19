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
    updateSelection();
}

void LyricsPlayer::updateLines() {
    int currentFirstLineIndexBefore = currentFirstLineIndex;

    const Lyrics::Line* firstLine = &lyrics->getLineAt(currentFirstLineIndex);
    if (firstLine->getEndSeek() < seek && currentFirstLineIndex < getDisplayLinesCount() - 1) {
        do {
            firstLine = &lyrics->getLineAt(++currentFirstLineIndex);
        } while(firstLine->getEndSeek() < seek && currentFirstLineIndex < getDisplayLinesCount() - 1);
    } else if (firstLine->startSeek > seek && currentFirstLineIndex != 0) {
        int i = currentFirstLineIndex;
        do {
            firstLine = &lyrics->getLineAt(--i);
        } while(i > 0 && firstLine->startSeek > seek);

        currentFirstLineIndex = i;
        if (firstLine->getEndSeek() < seek) {
            currentFirstLineIndex++;
        }
    }

    if (currentFirstLineIndexBefore != currentFirstLineIndex) {
        if (onLinesChanged) {
            onLinesChanged(this);
        }
    }
}

void LyricsPlayer::updateSelection() {
    const auto& line = lyrics->getLineAt(currentFirstLineIndex);

    Selection selection;
    if (seek > lyrics->getLastLine().getEndSeek()) {
        selection.lineSelection.charactersCount = lyrics->getLastLine().charactersCount;
        selection.lineSelection.lastCharacterSelectionPosition = 1.0;
        selection.lineIndex = 1;
    } else {
        selection.lineSelection = lyrics->getLineSelection(line, seek);
        assert(simultaneouslyLinesDisplayCount == 2 && "Hardcoded, pls fix");
        selection.lineIndex = currentFirstLineIndex == lyrics->getLinesCount() - 1 ? 1 : 0;
    }

    if (selection != this->selection) {
        this->selection = selection;
        onSelectionChanged(this->selection);
    }
}

int LyricsPlayer::getDisplayLinesCount() const {
    return simultaneouslyLinesDisplayCount;
}

std::u32string_view LyricsPlayer::getDisplayedLineAt(int index) const {
    assert(index < getDisplayLinesCount());
    return lyrics->getLineTextAt(index);
}

bool LyricsPlayer::Selection::operator==(const LyricsPlayer::Selection &rhs) const {
    return lineSelection == rhs.lineSelection && lineIndex == rhs.lineIndex;
}

bool LyricsPlayer::Selection::operator!=(const LyricsPlayer::Selection &rhs) const {
    return !(rhs == *this);
}
