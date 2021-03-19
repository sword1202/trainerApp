//
// Created by Semyon Tikhonenko on 2019-02-11.
//

#ifndef TEXTIMAGESGENERATOR_LYRICS_H
#define TEXTIMAGESGENERATOR_LYRICS_H

#include <map>
#include <vector>
#include <string>
#include <optional>

class Lyrics {
public:
    enum SectionType {
        CHORUS, VERSE, BRIDGE
    };

    struct Section {
        SectionType sectionType;
        int number;
        int startCharacterIndex;
    };

    struct Range {
        int startCharacterIndex;
        int charactersCount;
        double startSeek;
        double endSeek;
        const Section* section = nullptr;

        inline double getDuration() const {
            return endSeek - startSeek;
        }

        void writeToStream(std::ostream& os) const;
    };

    struct Line {
        int startCharacterIndex = 0;
        int charactersCount = 0;
        double startSeek;
        double duration;

        inline double getEndSeek() const {
            return startSeek + duration;
        }
    };

    struct LineSelection {
        int charactersCount = 0;
        // [0.0;1.0]
        double lastCharacterSelectionPosition = 0.0;

        inline bool operator == (const LineSelection& o) const {
            return charactersCount == o.charactersCount &&
            lastCharacterSelectionPosition == o.lastCharacterSelectionPosition;
        }

        inline bool operator != (const LineSelection& o) const {
            return charactersCount != o.charactersCount ||
                    lastCharacterSelectionPosition != o.lastCharacterSelectionPosition;
        }
    };

private:
    std::u32string text;
    std::vector<Section> sections;
    std::map<double, Range> ranges;
    std::vector<Line> lines;

    static std::pair<double, double> parseRange(const std::u32string, int begin, int end);
    static SectionType getSectionTypeByTypeId(char32_t sectionType);
    static char sectionTypeToChar(SectionType sectionType);

    std::u32string getRangeText(const Range& range) const;
    std::string getRangeUtf8Text(const Range& range) const;
public:
    static Lyrics EMPTY;

    const std::vector<Section>& getSections() const;
    const Line& getLineAt(int index) const;
    const Line& getLastLine() const;
    int getLinesCount() const;
    std::u32string_view getLineText(const Line& line) const;
    std::u32string_view getLineTextAt(int index) const;

    LineSelection getLineSelection(
            const Line& line,
            double seek) const;

    Lyrics() = default;
    Lyrics(const std::string& utf8);

    std::string toUtf8String() const;

    bool isEmpty() const;
};


#endif //TEXTIMAGESGENERATOR_LYRICS_H
