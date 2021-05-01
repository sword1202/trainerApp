#include "WorkspaceDrawerColorScheme.h"

const WorkspaceDrawerColorScheme* WorkspaceDrawerColorScheme::getDefault() {
    static WorkspaceDrawerColorScheme* scheme = nullptr;
    if (!scheme) {
        scheme = new WorkspaceDrawerColorScheme();
        scheme->gridColor = {0x8B, 0x89, 0xB6, 0x33};
        scheme->accentGridColor = {0x8B, 0x89, 0xB6, 0x80};
        scheme->pitchGraphColor = {0xFF, 0x5E, 0x85, 0xFF};
        scheme->pitchColor = {0x6E, 0x7E, 0xC5, 0xFF};
        scheme->borderLineColor = {0x8B, 0x89, 0xB6, 0xCC};
        scheme->boundsColor = {0xC4, 0xCD, 0xFD, 0xFF};
        scheme->playHeadColor = {0x24, 0x23, 0x2D, 0xFF};
        scheme->instrumentalTrackColor = {0x97, 0x98, 0xB5, 0xFF};
        scheme->pianoTrackColor = Color::white();
        scheme->pianoTrackShadowColor = {0xDD, 0xDB, 0xEE, 0x99};
        scheme->pianoTrackPitchesColor = {0x51, 0x4E, 0x64, 0xFF};
        scheme->endingColor = {0xFF, 0x5E, 0x85, 0xFF};
    }

    return scheme;
}