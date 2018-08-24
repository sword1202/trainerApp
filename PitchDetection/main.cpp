// Simple example program that records using an audio queue,
// prints the value of one sample from each resultBuffer recorded,
// and attempts to stop and restart the recording every two
// seconds.
//
// This can be built from the command line with
//
//  g++ rectest.cc -framework CoreFoundation -framework AudioToolbox -framework CoreAudio -o rectest
//

#include "AudioInputReader.h"
#include "PitchInputReader.h"
#include "WavFileAudioInputReader.h"
#include "WavFilePitchesReader.h"
#include "AubioPitchDetector.h"
#include <iostream>

using namespace std;

const char* WAV_FILE = "assets/3285.wav";

void runPitchDetection(AudioInputReader *audioInputReader) {
    PitchInputReader pitchInputReader(audioInputReader, new AubioPitchDetector(), 1);
    pitchInputReader.setExecuteCallBackOnInvalidPitches(true);
    pitchInputReader.setCallback([](Pitch pitch) {
        cout<<pitch<<endl;
    });
    pitchInputReader.getPitchDetector()->setThreshold(0.15);
    pitchInputReader.start();
    char key;
    while(true) {
        cin>>key;
        if (key != 'c') {
            break;
        }
    };
    pitchInputReader.stop();
}

int main(int argc, char **argv) {
    cout<<"Choose option: a - read from microphone, b - from 3285.wav sequentially, c - from 3285.wav immediately\n";
    char option = 'a';
    //cin>>option;
    cout<<"Option accepted\n";
    if (option == 'a' || option == 'b') {
        if (option == 'a') {
            AudioInputReader* audioInputReader = CreateDefaultAudioInputReader(1024);
            runPitchDetection(audioInputReader);
        } else {
            std::fstream* wavFile = new fstream(WAV_FILE);
            WavFileAudioInputReader* wavAudioInputReader = new WavFileAudioInputReader(wavFile, 1200);
            wavAudioInputReader->setDestroyStreamOnDestructor(true);
            runPitchDetection(wavAudioInputReader);
        }
    } else if(option == 'c') {
        WavFilePitchesReader wavFilePitchesReader;
        std::fstream wavFile(WAV_FILE);
        vector<PitchDetection> pitches = wavFilePitchesReader.readPitchesFromWavFile(&wavFile, 0.2f);
        ostream_iterator<Pitch> iter(cout, "\n");
        std::transform(pitches.begin(), pitches.end(), iter, [](const PitchDetection& detection) {
            return detection.pitch;
        });
    } else {
        cout<<"Option not supported\n";
    }

    return 0;
}