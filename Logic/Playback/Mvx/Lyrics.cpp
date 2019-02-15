//
// Created by Semyon Tikhonenko on 2019-02-11.
//

#include "Lyrics.h"
#include "StringUtils.h"

using namespace CppUtils;

bool LyricsInterval::containsTime(double time) const {
    return time >= startTime && time <= endTime;
}

int Lyrics::getLinesCount() const {
    return int(lyrics.size());
}

const std::string &Lyrics::getCurrentLyricsTextAtLine(int lineIndex, double time) const {
    assert(lineIndex < lyrics.size());
    const LyricsLine& line = lyrics[lineIndex];
    for (const LyricsInterval& interval : line.intervals) {
        if (interval.containsTime(time)) {
            return interval.text;
        }
    }

    return Strings::EMPTY;
}

const std::string& Lyrics::getLineName(int index) const {
    assert(index < lyrics.size());
    return lyrics[index].name;
}

Lyrics::Lyrics(const LyricsLine &lyricsLine) {
    lyrics.push_back(lyricsLine);
}

Lyrics::Lyrics(const LyricsLine &lyricsLine1, const LyricsLine &lyricsLine2) {
    lyrics.push_back(lyricsLine1);
    lyrics.push_back(lyricsLine2);
}
