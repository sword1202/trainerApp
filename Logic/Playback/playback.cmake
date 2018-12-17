set(playbackSources
        Base/AudioPlayer.cpp
        Base/AudioPlayerWithDefaultSeekHandler.cpp
        Base/PlaybackBounds.cpp
        Base/PortAudioUtils.cpp

        Decoding/AudioFilePlayer.cpp
        Decoding/Decoder/audiodecoder.cpp

        Midi/MidiFileReader.cpp
        Midi/MidiTrack.cpp
        Midi/CraigsappMidifile/Binasc.cpp
        Midi/CraigsappMidifile/MidiEvent.cpp
        Midi/CraigsappMidifile/MidiEventList.cpp
        Midi/CraigsappMidifile/MidiFile.cpp
        Midi/CraigsappMidifile/MidiMessage.cpp

        Mvx/MvxFile.cpp
        Mvx/MvxPlayer.cpp

        Other/MetronomeAudioPlayer.cpp
        Other/RealtimeStreamingAudioPlayer.cpp
        Other/WavAudioPlayer.cpp
        Other/BaseRawPcmAudioDataPlayer.cpp

        Vx/VxFile.cpp
        Vx/VxFileAudioDataGenerator.cpp
        Vx/VxFileAudioPlayer.cpp)

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
