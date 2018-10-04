set(playbackSources
        Base/AudioPlayer.cpp
        Base/AudioPlayerWithDefaultSeekHandler.cpp
        Base/PlaybackBounds.cpp

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

        Other/BaseWavAudioPlayer.cpp
        Other/MetronomeAudioPlayer.cpp
        Other/RealtimeStreamingAudioPlayer.cpp
        Other/WavAudioPlayer.cpp

        Vx/VxFile.cpp
        Vx/VxFileAudioDataGenerator.cpp
        Vx/VxFileAudioPlayer.cpp)

if (APPLE)
    list(APPEND playbackSources Decoding/Decoder/audiodecodercoreaudio_mac.cpp)
endif(APPLE)