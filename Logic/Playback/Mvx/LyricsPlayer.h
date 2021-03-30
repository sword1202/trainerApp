//
// Created by Semyon Tikhonenko on 2/27/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_LYRICSPLAYER_H
#define VOCALTRAINER_LYRICSPLAYER_H

#include "Lyrics.h"
#include "LyricsDisplayedLinesProvider.h"
#include <vector>
#include <functional>

class LyricsPlayer : public LyricsDisplayedLinesProvider {
public:
    struct Selection  {
        Lyrics::LineSelection lineSelection;
        int lineIndex = 0;

        bool operator==(const Selection &rhs) const;

        bool operator!=(const Selection &rhs) const;
    };
private:
    const Lyrics* lyrics;
    double seek = 0;
    int simultaneouslyLinesDisplayCount;
    int currentOrNextLineIndex = 0;
    Selection selection;

    void updateLines();
    void updateSelection();

    int convertDisplayedLineIndexIntoGlobalIndex(int index) const;
public:
    std::function<void(const LyricsDisplayedLinesProvider*)> onLinesChanged;
    std::function<void(const Selection&)> onSelectionChanged;

    LyricsPlayer(const Lyrics *lyrics, int simultaneouslyLinesDisplayCount = 2);

    double getSeek() const;
    void setSeek(double seek);

    int getDisplayLinesCount() const override;
    std::u32string_view getDisplayedLineAt(int index) const override;
};


#endif //VOCALTRAINER_LYRICSPLAYER_H
