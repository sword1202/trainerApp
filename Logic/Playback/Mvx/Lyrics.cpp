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

int Lyrics::getNumberOfParts() const {
    return int(lyrics.size());
}

const std::string &Lyrics::getCurrentLyricsTextForPart(int partIndex, double time) const {
    assert(partIndex < lyrics.size());
    const LyricsPart& line = lyrics[partIndex];
    for (const LyricsInterval& interval : line.intervals) {
        if (interval.containsTime(time)) {
            return interval.text;
        }
    }

    return Strings::EMPTY;
}

const std::string& Lyrics::getPartName(int index) const {
    assert(index < lyrics.size());
    return lyrics[index].name;
}

Lyrics::Lyrics(const LyricsPart &lyricsLine) {
    lyrics.push_back(lyricsLine);
}

Lyrics::Lyrics(const LyricsPart &lyricsLine1, const LyricsPart &lyricsLine2) {
    lyrics.push_back(lyricsLine1);
    lyrics.push_back(lyricsLine2);
}

const LyricsPart& Lyrics::getLyricsPart(int index) {
    assert(index < getNumberOfParts());
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

Lyrics::Snapshot Lyrics::getCurrentSnapshot(double time) const {
    Snapshot result;
    for (int i = 0; i < getNumberOfParts(); ++i) {
        result.push_back(getCurrentLyricsTextForPart(i, time));
    }

    return result;
}
