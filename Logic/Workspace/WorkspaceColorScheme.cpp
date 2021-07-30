#include "WorkspaceColorScheme.h"

WorkspaceColorScheme::WorkspaceColorScheme() {
    gridColor = Color::fromRgba(0x8B89B633);
    accentGridColor = Color::fromRgba(0x8B89B680);
    pitchGraphColor = Color::fromRgba(0xFF5E85FF);
    pitchColor = Color::fromRgba(0x6E7EC5FF);
    pianoSharpPitchColor = Color::fromRgba(0x9A98D0ff);
    borderLineColor = Color::fromRgba(0x8B89B6CC);
    boundsColor = Color::fromRgba(0xC4CDFDFF);
    playHeadColor = Color::fromRgba(0x24232DFF);
    instrumentalTrackColor = Color::fromRgba(0x9798B5FF);
    pianoTrackColor = Color::white();
    pianoTrackShadowColor = Color::fromRgba(0xDDDBEE99);
    pianoTrackPitchesColor = Color::fromRgba(0x514E64FF);
    endingColor = Color::fromRgba(0xFF5E85FF);
    pianoBorderColor = Color::fromRgba(0x9A98D07f);
    pianoSelectedPitchColor = Color::fromRgba(0x615F97ff);
}
