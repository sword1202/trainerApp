//
// Created by Semyon Tikhonenko on 2019-02-11.
//

#include "Lyrics.h"
#include "Algorithms.h"
#include "StringEncodingUtils.h"
#include "StringUtils.h"
#include "Maps.h"
#include "MathUtils.h"

using namespace CppUtils;

Lyrics::Lyrics(const std::string &utf8) {
    std::u32string u32 = UtfUtils::ToUtf32(utf8);
    u32 = Strings::LeftTrim(u32);
    u32 = Strings::RemoveAllCharacterRepetitions(u32, U'\n');

    text.reserve(u32.size());
    int skippedCharactersCount = 0;
    const Range* previousRange = nullptr;
    int rangeStartCharacter = -1;
    Line* currentLine = nullptr;
    Section* currentSection = nullptr;

    for (int i = 0; i < u32.size(); ++i) {
        char32_t ch = u32[i];
        if (ch == U'[') {
            // Parse range description
            size_t rangeEndIndex = u32.find(']', static_cast<size_t>(i + 1));
            if (rangeEndIndex == std::string::npos) {
                throw std::runtime_error("Lyrics parse failed, can't find ] for range description");
            }

            auto parsedRange = parseRange(u32, i + 1, static_cast<int>(rangeEndIndex));
            Range range;

            if (rangeStartCharacter < 0) {
                throw std::runtime_error("Lyrics parse failed, range doesn't contain any characters");
            }

            assert(currentSection);
            range.startCharacterIndex = rangeStartCharacter - skippedCharactersCount;
            range.charactersCount = i - rangeStartCharacter;
            range.startSeek = parsedRange.first;
            range.endSeek = parsedRange.second;
            range.section = currentSection;

            if (previousRange && previousRange->endSeek > range.startSeek) {
                std::stringstream message;
                message << "Lyrics parse failed, invalid range: ["
                << range.startSeek << ";" << range.endSeek << "] "
                << "after range: [" << previousRange->startSeek << ";" << previousRange->endSeek << "]";
                throw std::runtime_error(message.str());
            }

            previousRange = Maps::InsertAndGetInsertedValue(ranges, range.startSeek, range);

            if (!currentLine) {
                Line line;
                line.startSeek = range.startSeek;
                line.startCharacterIndex = range.startCharacterIndex;
                line.charactersCount = range.charactersCount;
                line.duration = range.getDuration();
                lines.push_back(line);
                currentLine = &lines.back();
            } else {
                currentLine->duration = (range.startSeek - currentLine->startSeek) + range.getDuration();
                currentLine->charactersCount = (range.startCharacterIndex - currentLine->startCharacterIndex) +
                        range.charactersCount;
            }

            assert(currentSection);
            if (currentSection->seek < 0) {
                currentSection->seek = range.startSeek;
            }

            skippedCharactersCount += (rangeEndIndex - i + 1);
            i = static_cast<int>(rangeEndIndex);
            rangeStartCharacter = -1;
        } else if (ch == U'{') {
            if (!u32.empty() && u32[i - 1] != U'\n' && i != 0) {
                throw std::runtime_error("Lyrics parse error, section description should be at a new line");
            }

            // Parse section
            size_t sectionEndIndex = u32.find('}', static_cast<size_t>(i + 1));
            if (sectionEndIndex == std::string::npos) {
                throw std::runtime_error("Lyrics parse failed, can't find } for section description");
            }
            char32_t sectionTypeId = u32[i + 1];
            Section section;
            section.sectionType = getSectionTypeByTypeId(sectionTypeId);
            section.number = Strings::ParseUnsignedInt(u32, i + 2, static_cast<int>(sectionEndIndex));
            if (section.number < 0) {
                throw std::runtime_error("Failed to parse section description: " +
                        UtfUtils::ToUtf8(text.substr(static_cast<size_t>(i),
                                static_cast<size_t>(sectionEndIndex - i + 1))) + ", invalid section number");
            }

            skippedCharactersCount += (sectionEndIndex - i + 1) + 1; // +1 new line
            i = static_cast<int>(sectionEndIndex) + 1;
            sections.push_back(section);
            currentSection = &sections.back();
        } else {
            text.push_back(ch);
            if (ch == U'\n') {
                currentLine = nullptr;
            } else {
                if (rangeStartCharacter < 0) {
                    rangeStartCharacter = i;
                }
            }
        }
    }
}

std::pair<double, double> Lyrics::parseRange(const std::u32string text, int begin, int end) {
    bool success = false;
    auto split = Strings::SplitDoubles(text, begin, end, U';', &success);
    if (split.size() != 2 || !success) {
        throw std::runtime_error("Failed to parse range description: " +
        UtfUtils::ToUtf8(text.substr(static_cast<size_t>(begin), static_cast<size_t>(end - begin))));
    }

    return std::make_pair(split[0], split[1]);
}

Lyrics::SectionType Lyrics::getSectionTypeByTypeId(char32_t sectionType) {
    switch (sectionType) {
        case U'C':
            return CHORUS;
        case U'V':
            return VERSE;
        case U'B':
            return BRIDGE;
        default:
            throw std::runtime_error("Lyrics parse failed, unknown section type" +
            UtfUtils::ToUtf8(std::u32string(sectionType, 1)));
    }
}

char Lyrics::sectionTypeToChar(Lyrics::SectionType sectionType) {
    switch (sectionType) {
        case CHORUS:
            return 'C';
        case VERSE:
            return 'V';
        case BRIDGE:
            return 'B';
        default:
            assert(false);
            return 'U';
    }
}

std::string Lyrics::toUtf8String() const {
    std::stringstream result;

    auto rangeIter = ranges.begin();
    auto lineIter = lines.begin();
    for (const Section& section : sections) {
        result << '{' << sectionTypeToChar(section.sectionType) << section.number << "}\n";
        const Range* range = &rangeIter->second;
        while (range->section == &section) {
            if (lineIter->startCharacterIndex + lineIter->charactersCount < range->startCharacterIndex) {
                result << "\n";
                ++lineIter;
            }

            result << getRangeUtf8Text(*range);
            range->writeToStream(result);
            if (++rangeIter == ranges.end()) {
                break;
            }

            range = &rangeIter->second;
        }
    }

    return result.str();
}

std::u32string Lyrics::getRangeText(const Lyrics::Range &range) const {
    return text.substr(static_cast<size_t>(range.startCharacterIndex), static_cast<size_t>(range.charactersCount));
}

std::string Lyrics::getRangeUtf8Text(const Lyrics::Range &range) const {
    return UtfUtils::ToUtf8(getRangeText(range));
}

const std::vector<Lyrics::Section> &Lyrics::getSections() const {
    return sections;
}

int Lyrics::getLinesCount() const {
    return static_cast<int>(lines.size());
}

const Lyrics::Line& Lyrics::getLineAt(int index) const {
    assert(index < lines.size());
    return lines[index];
}

std::u32string_view Lyrics::getLineText(const Line& line) const {
    auto begin = text.data() + line.startCharacterIndex;
    return std::u32string_view(begin, static_cast<size_t>(line.charactersCount));
}

std::u32string_view Lyrics::getLineTextAt(int index) const {
    return getLineText(getLineAt(index));
}

Lyrics::LineSelection Lyrics::getLineSelection(
        const Lyrics::Line &line,
        double seek) const
{
    LineSelection selection;
    auto upper = ranges.upper_bound(seek);
    if (upper == ranges.begin()) {
        return selection;
    }

    const Range& range = (--upper)->second;
    if (range.endSeek < seek) {
        return selection;
    } else {
        double characterDuration = range.getDuration() / range.charactersCount;
        double seekInRange = seek - range.startSeek;
        double f = fmod(seekInRange, characterDuration);
        if (f != 0) {
            selection.lastCharacterSelectionPosition = f / characterDuration;
        } else {
            selection.lastCharacterSelectionPosition = 1.0;
        }
        selection.charactersCount =  Math::RoundToInt((seekInRange - f) / characterDuration)
                + range.startCharacterIndex - line.startCharacterIndex;
        if (f != 0) {
            selection.charactersCount++;
        }

        return selection;
    }
}

bool Lyrics::isEmpty() const {
    return text.empty();
}

const Lyrics::Line &Lyrics::getLastLine() const {
    return getLineAt(getLinesCount() - 1);
}

void Lyrics::Range::writeToStream(std::ostream &os) const {
    os << '[' << startSeek << ';' << endSeek << ']';
}

Lyrics Lyrics::EMPTY;
