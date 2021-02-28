//
// Created by Semyon Tikhonenko on 2/28/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_LYRICSDISPLAYEDLINESPROVIDER_H
#define VOCALTRAINER_LYRICSDISPLAYEDLINESPROVIDER_H


class LyricsDisplayedLinesProvider {
public:
    virtual int getDisplayLinesCount() const = 0;
    virtual std::u32string_view getDisplayedLineAt(int index) const = 0;
    virtual ~LyricsDisplayedLinesProvider() = default;
};


#endif //VOCALTRAINER_LYRICSDISPLAYEDLINESPROVIDER_H
