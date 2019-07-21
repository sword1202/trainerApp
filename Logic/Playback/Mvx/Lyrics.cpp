//
// Created by Semyon Tikhonenko on 2019-02-11.
//

#include "Lyrics.h"
#include "StringUtils.h"

using namespace CppUtils;

bool LyricsInterval::containsTime(double time) const {
    return time >= startTime && time <= endTime;
}

bool LyricsInterval::operator==(const LyricsInterval &rhs) const {
    return startTime == rhs.startTime &&
           endTime == rhs.endTime &&
           text == rhs.text;
}

bool LyricsInterval::operator!=(const LyricsInterval &rhs) const {
    return !(rhs == *this);
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

const LyricsLine& Lyrics::getLyricsLine(int index) {
    assert(index < getLinesCount());
    return lyrics[index];
}

void Lyrics::addLyricsInterval(int lineIndex, const LyricsInterval& interval) {
    lyrics.resize(lineIndex + 1);
    lyrics[lineIndex].intervals.push_back(interval);
}

bool Lyrics::operator==(const Lyrics &rhs) const {
    return lyrics == rhs.lyrics;
}

bool Lyrics::operator!=(const Lyrics &rhs) const {
    return !(rhs == *this);
}
