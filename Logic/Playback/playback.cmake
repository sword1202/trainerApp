set(midiSources
        Midi/MidiFileReader.cpp
        Midi/MidiTrack.cpp
        Midi/MidiFileReaderException.cpp
        Midi/MidiFileReaderException.h
        Midi/CraigsappMidifile/Binasc.cpp
        Midi/CraigsappMidifile/MidiEvent.cpp
        Midi/CraigsappMidifile/MidiEventList.cpp
        Midi/CraigsappMidifile/MidiFile.cpp
        Midi/CraigsappMidifile/MidiMessage.cpp
        )

set(vxMvxFileSources
        Mvx/MvxFile.cpp
        Mvx/MvxFile.h
        Vx/VocalPart.cpp
        )

set(playbackSources
        Base/AudioPlayer.cpp
        Base/AudioPlayerWithDefaultSeekHandler.cpp
        Base/PlaybackBounds.cpp
        Base/PortAudioUtils.cpp
        Base/PrepareFailedException.cpp
        Base/PrepareFailedException.h

        Decoding/AudioFilePlayer.cpp
        Decoding/Decoder/audiodecoder.cpp
        Decoding/Decoder/DecodedTrack.h
        Decoding/Decoder/DecodedTrack.cpp

        ${midiSources}
        ${vxMvxFileSources}
        ${midiSources}

        Mvx/MvxPlayer.cpp
        Mvx/MvxPlayer.h
        Mvx/Lyrics.cpp
        Mvx/MvxPlayerPrepareException.cpp
        Mvx/MvxPlayerPrepareException.h

        Vx/VocalPartAudioDataGenerator.cpp
        Vx/VocalPartAudioPlayer.cpp

        Other/MetronomeAudioPlayer.cpp
        Other/RealtimeStreamingAudioPlayer.cpp
        Other/WavAudioPlayer.cpp
        Other/BaseRawPcmAudioDataPlayer.cpp)

set(playbackTestSources
        ${midiSources}
        ${vxMvxFileSources}
        ${midiSources}

        Vx/VocalPartAudioDataGenerator.cpp

        Decoding/Decoder/audiodecoder.cpp
        Decoding/Decoder/audiodecodercoreaudio_mac.cpp
        )

if (APPLE)
    list(APPEND playbackSources Decoding/Decoder/audiodecodercoreaudio_mac.cpp)
endif(APPLE)

if (WIN32)
    list(APPEND playbackSources Decoding/Decoder/audiodecodermediafoundation_win.cpp)
endif(WIN32)

if (LINUX)
    list(APPEND playbackSources Decoding/Decoder/audiodecoderffmpeg.cpp)
endif(LINUX)

list(TRANSFORM playbackSources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
list(TRANSFORM playbackTestSources PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
